// -----------------------------------------------------------------------------
// Module: module_include
// Gorka Hernández Villalón
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "./module_include.h"
#include "../module_parser/module_parser.h"
#include "../module_errors/module_errors.h"
static int include_depth = 0;
void module_include_run(void) {
    printf("Loaded module_include\n");
}

//Se come los caracteres hasta encontrar el delimitador del cierre o un salto de lineas
static int read_filename(ParserState* state, char* out, size_t max, char end) {
    size_t i = 0;
    char c;
    while ((c = read_char(state)) && c != '\n') {
        if (c == end) {
            out[i] = '\0';
            return 1;
        }
        if (i >= max - 1) return 0;
        out[i++] = c;
    }
    return 0;
}

//Llama a fopen con tal de abrir el archivo, en caso contrario constuye la ruta relativa al fichero
//actual concatenando el nmbre y vuelve el open
static FILE* open_include(ParserState* state, const char* name) {
    FILE* f = fopen(name, "r");
    if (f) return f;
    const char* slash = strrchr(state->current_filename, '/');
    if (!slash) return NULL;
    char path[MAX_INCLUDE_PATH];
    size_t len = slash - state->current_filename + 1;
    if (len + strlen(name) >= MAX_INCLUDE_PATH) return NULL;
    strncpy(path, state->current_filename, len);
    path[len] = '\0';
    strcat(path, name);
    return fopen(path, "r");
}

//Se abre el archivo mediante open_include y en caso que no se pueda abrir hace un report_error
static int insert_file(ParserState* state, const char* name, bool copy) {
    FILE* f = open_include(state, name);
    if (!f) {
        report_error(ERROR_ERROR, state->current_filename, state->current_line, "Cannot open included file");
        return 0;
    }
    if (copy && state->output_file)
        fprintf(state->output_file, "\n// Begin included file: %s\n", name);
    int c;
    while ((c = fgetc(f)) != EOF)
        if (copy && state->output_file)
            fputc(c, state->output_file);
    if (copy && state->output_file)
        fprintf(state->output_file, "\n// End included file: %s\n", name);
    fclose(f);
    return 1;
}

//Función de entrada es llamada cuando el parser detecta un #, se llamará con el parser state
//Para extraer el nombre se llama a read_filename y una vez tiene el file_name se llama a insert_file
int process_include(ParserState* state, char current_char, bool copy) {
    if (current_char != '#') return 0;
    char directive[32] = {0};
    int i = 0;
    char c;
    while ((c = peek_char(state)) == ' ' || c == '\t')
        read_char(state);
    while (isalpha(peek_char(state)) && i < 31)
        directive[i++] = read_char(state);
    if (strcmp(directive, "include") != 0)
        return 0;
    if (include_depth >= MAX_INCLUDE_DEPTH) {
        report_error(ERROR_ERROR, state->current_filename, state->current_line, "Maximum include depth exceeded");
        return 1;
    }
    while ((c = peek_char(state)) == ' ' || c == '\t')
        read_char(state);
    c = read_char(state);
    char end = (c == '"') ? '"' : (c == '<') ? '>' : 0;
    if (!end) {
        report_error(ERROR_ERROR, state->current_filename, state->current_line,"Invalid #include syntax");
        return 1;
    }
    char filename[MAX_INCLUDE_PATH];
    if (!read_filename(state, filename, sizeof(filename), end))
        return 1;
    while ((c = read_char(state)) && c != '\n');
    include_depth++;
    if (copy) {
        if (state->process_directives)
            insert_file(state, filename, copy);
        else
            fprintf(state->output_file, "#include %c%s%c\n", end == '"' ? '"' : '<', filename, end);
    }
    include_depth--;
    return 1;
}