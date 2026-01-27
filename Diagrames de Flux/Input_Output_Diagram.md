# Diagrama Input/Output dels Mòduls del Preprocessador

## Flux General 

```
+---------------------+
|  User / Command Line |
+----------+----------+
           |
           v (argc, argv)
+---------------------------------------------------------------+
|                        MAIN PROGRAM                           |
|                        (src/main.c)                           |
|  1. Init Errors                                               |
|  2. Process Args  <----->  [ Module Args ]                    |
|  3. Init Parser                                               |
|  4. Run Loop      ------>  [ Module Parser ]                  |
+---------------------------------------------------------------+
                                  |
                                  v (ParserState)
+---------------------------------------------------------------+
|                      MODULE PARSER                            |
|                  (src/module_parser.c)                        |
|                                                               |
|   +-------------------+      +----------------------------+   |
|   |   Input Stream    |----->|   Token Detection Logic    |   |
|   +-------------------+      +-------------+--------------+   |
|                                            |                  |
|          +---------punter enviat apuntant dsp de:--+          |
|          |                 |               |       |          |
|   (// or /*)        (#define)        (#include)   (#ifdef)    |
| +----------------+ +-------------+ +----------+ +-----------+ |
| | Module Comments| |Module Define| |Mod Include| |Mod Ifdef | |
| +----------------+ +-------------+ +----------+ +-----------+ |
|          |                 |              |           |       |
|          +-----------------+--------------+-----------+       |
|                            |                                  |
|                            v                                  |
|                  +-------------------+                        |
|                  |  copy output file |                        |
|                  |  if need it       |                        |
|                  +-------------------+                        |
+---------------------------------------------------------------+
           |
           v
    +-------------+
    | Output File |
    +-------------+
```
# Flux Arguments 

```
INPUT: .exe -flag file.c
         ↓
    [MODULE ARGS]
         ↓
OUTPUT: ArgFlags + Input/Output filenames
```

---

## Detall per Mòdul

### 1. **MODULE ARGS** (module_arguments)
**Entrada:**
- `argc, argv` (arguments de la línia de comandaments)
- Exemple: `.exe -flag file.c `

**Sortida:**
- `ArgFlags struct` amb:
  - `ifile`: nom del fitxer d'entrada
  - `ofile`: nom del fitxer de sortida
  - `remove_comments`: flag -c (bool)
  - `process_directives`: flag -d (bool)
  - `show_help`: flag -help (bool)

**No retorna res al fitxer**, escriu missatges de debug al `stdout`

---

### 2. **PARSER** (module_parser)
**Entrada:**
- `ifile` (fitxer d'entrada)
- `ofile` (fitxer de sortida)
- `ArgFlags` (flags de configuració)

**Sortida:**
- Cap de directa. Escriu el fitxer processat a `ofile`
- `ParserState` (estat intern del parser)

**Responsabilitats:**
- Llegeix el fitxer caràcter per caràcter
- Detecta directives preprocessador
- Delega a mòduls especificats según la directiva trobada

---

### 3. **MODULE COMMENTS_REMOVE** (procés dins del parser)
**Entrada:**
- Caràcters llegits pel parser
- `current_char` i `next_char` (per detectar `//` o `/*`)

**Sortida:**
- No retorna res
- Escriu al fitxer de sortida:
  - Si `-c` flag: escriu els comentaris (preservats)
  - Si `-c` no està actiu: els comments es descarten

---

### 4. **MODULE INCLUDE** (procés dins del parser)
**Entrada:**
- Detecta directiva `#include "file"` o `#include <file>`
- Llegeix fins trobar `"` o `>`
- Recursiu: pot processar directives dins del fitxer incluït

**Sortida:**
- No retorna res
- Escriu al fitxer de sortida:
  - El contingut del fitxer incluït (processat recursivament)
  - Sense la línea `#include` original

**Exemple:**
```c
// Input: file.c
#include "header.h"
int main() {}

// Sortida a output.c:
[contingut de header.h aquí]
int main() {}
```

---

### 5. **MODULE DEFINE** (procés dins del parser)
**Entrada:**
- Detecta directiva `#define MACRO valor`
- Llegeix el nom del macro i el seu valor

**Sortida:**
- No retorna res
- Emmagatzema el macro a `MacroDict`
- No escriu al fitxer de sortida (la directiva `#define` es descarta)
- Usa el macro per a substitucions posteriores

**Exemple:**
```c
// Input: 
#define PI 3.14159
float area = PI * r * r;

// Sortida:
float area = 3.14159 * r * r;
```

---

### 6. **MODULE IFDEF/ENDIF** (procés dins del parser)
**Entrada:**
- Detecta `#ifdef MACRO` o `#ifndef MACRO`
- Comprova a `MacroDict` si el macro està definit
- Processa recursivament fins trobar `#else` o `#endif`

**Sortida:**
- No retorna res
- Escriu al fitxer de sortida:
  - Si la condició és certa: escriu el bloc `#ifdef`
  - Si la condició és falsa: descarta el bloc (o escriu el `#else`)
- Les directives `#ifdef`, `#else`, `#endif` NO es copien

**Exemple:**
```c
// Input:
#define DEBUG
#ifdef DEBUG
    printf("Debug mode");
#endif

// Sortida:
    printf("Debug mode");
```

---

## Flux Complet d'Exemple

```
INPUT: preprocessor input.c output.c -c -d

                          ┌──────────────────────┐
                          │   MODULE ARGS        │
                          │ processa: -c -d      │
                          │ input: input.c       │
                          │ output: output.c     │
                          └──────────┬───────────┘
                                     │
                                     ▼
                          ┌──────────────────────┐
                          │   PARSER INIT        │
                          │ obri fitxers         │
                          │ crea ParserState     │
                          └──────────┬───────────┘
                                     │
                                     ▼
                          ┌──────────────────────────────────┐
                          │   PARSE UNTIL (main loop)        │
                          │ llegeix char per char            │
                          │                                  │
                          │  Detecta:                        │
                          │  - '//' o '/*' → COMMENTS        │
                          │  - '#include' → INCLUDE          │
                          │  - '#define' → DEFINE            │
                          │  - '#ifdef' → IFDEF/ENDIF        │
                          │  - altres → escriu output        │
                          │                                  │
                          │ Crides recursives als mòduls    │
                          └──────────┬───────────────────────┘
                                     │
                    ┌────┬────┬──────┴──────┬────┐
                    ▼    ▼    ▼             ▼    ▼
            ┌──────────────────┐  ┌──────────────────┐
            │COMMENTS_REMOVE   │  │  INCLUDE         │
            │Si -c: preserva   │  │  Lee: "file.h"   │
            │Si -c no: descarta│  │  Processa        │
            │No retorna res    │  │  recursivament   │
            │Escriu output     │  │  No retorna res  │
            │                  │  │  Escriu output   │
            └──────────────────┘  └──────────────────┘
            ┌──────────────────┐  ┌──────────────────┐
            │  DEFINE          │  │  IFDEF/ENDIF     │
            │Lee: MACRO valor  │  │  Check MacroDict │
            │No retorna res    │  │  Si cert: inclou │
            │Guarda MacroDict  │  │  Si fals: descarta
            │No escriu output  │  │  No retorna res  │
            │(descarta línea)  │  │  Escriu output   │
            └──────────────────┘  └──────────────────┘
                                     │
                                     ▼
                          ┌──────────────────────────┐
                          │   OUTPUT FILE GENERATED  │
                          │   (output.c)             │
                          │   - Sense comentaris     │
                          │   - Sense #include       │
                          │   - Sense #ifdef blocs   │
                          │   - Macros substituïts   │
                          └──────────────────────────┘
```

---

## Taula Resum

| Mòdul | Input | Output | Escriu? | Retorna |
|-------|-------|--------|---------|---------|
| **ARGS** | argc, argv | ArgFlags | Sí (stdout) | ArgFlags* |
| **PARSER** | ifile, ofile, flags | ParserState | Sí (output file) | ParserState* |
| **COMMENTS** | chars (/, *) | - | Sí/No (segons flag) | int (0/1) |
| **INCLUDE** | "#include..." | - | Sí (content) | int |
| **DEFINE** | "#define..." | MacroDict | Sí (MacroDict) | int |
| **IFDEF** | "#ifdef..." | - | Sí (if block) | int |
| **ERRORS** | Errors | - | Sí (stderr) | - |

---

## Notes Importants

1. **Cap mòdul retorna dades pròpiament dit** - Tot s'escriu als fitxers o es guarda a estructures globals
2. **El parser és el "controlador"** - Llija el fitxer i delega a cada mòdul
3. **Els mòduls NO retornen strings o arrays** - Retornen códis d'error (int)
4. **La MacroDict és compartida** - Tots els mòduls accedeixen a la mateixa `ParserState->macro_dict`
5. **Les directives no van al output** - `#include`, `#define`, `#ifdef` es processen però NO es copien al fitxer de sortida



