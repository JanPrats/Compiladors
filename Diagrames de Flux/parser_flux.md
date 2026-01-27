# Flux del `parse_until()`

Aquest document descriu el flux de control de la funció parse_until() del parser, incloent el tractament de directives, comentaris, cadenes i substitució de macros.

## Diagrama de flux

```
┌───────────────────────┐
│ Inici parse_until()   │
└─────────┬─────────────┘
          │
          ▼
┌───────────────────────┐
│ Llegir caràcter c     │
│ read_char()           │
└─────────┬─────────────┘
          │
          ▼
┌─────────Si c == '\0'?─────────┐
│                               │
Sí ▼                              No ▼
┌────────────┐             ┌─────────────────┐
│ Retorna 0  │             │Estem al principi │
└────────────┘             │ de línia?        │
                           │at_line_start=true│
                           └─────────┬────────┘
                                     │
                                     ▼
          ┌─────────Si c == '#' i directives habilitades?────┐
          │                                                   │
         Sí ▼                                                 No ▼
┌────────────────────────────┐        ┌─────────────────────────────┐
│ Salta espais               │        │ c == '/' ?                  │
│ Llegeix directiva          │        └─────────────┬─────────────┘
└─────────────┬──────────────┘                      │
              │                                     ▼
              ▼                       ┌───── Comentari? ─────┐
┌─────────Compara directiva─────────┐ │ Sí─process_comment() │
│ include / define / ifdef / ...    │ │ No ─ continua        │
└─────────────┬────────────────────┘  └─────────┬───────────┘
              │                                 ▼
              ▼                      ┌────────c == '"' ?────────┐
┌─────────Directiva #include?────────┐│ Sí → Copiar literal      │
│ Sí → process_include()             ││      copy_to_output     │
│ No → #define?                      │└─────────┬──────────────┘
│ Sí → process_define()              │          ▼
│ No → #ifdef / #ifndef?             │┌────────Identificador?──────┐
│ Sí → process_ifdef()               ││ Sí → llegir paraula        │
│ No → #else / #endif / stop_symbol? ││ Substitueix macro si existeix│
│ Sí → retorna 0 o error             ││ substitute_macro()         │
└───────────────────────────────────┘└─────────┬──────────────────┘
                                                ▼
                                  ┌─────────Altres caràcters────────┐
                                  │ Copia a sortida si cal          │
                                  │ Actualitza at_line_start        │
                                  └─────────┬──────────────────────┘
                                            │
                                            ▼
                                   ┌───────────────┐
                                   │Tornar al bucle │
                                   └────────────────┘
