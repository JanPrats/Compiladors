#ifndef MODULE_INCLUDE_H
#define MODULE_INCLUDE_H

#include "../main.h"
#include <stdbool.h>

typedef struct ParserState ParserState;

#define MAX_INCLUDE_DEPTH 64
#define MAX_INCLUDE_PATH 512

int process_include(ParserState* state, char current_char, bool copy_to_output);
void module_include_run(void);

#endif
