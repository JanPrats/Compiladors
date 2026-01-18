#ifndef MODULE_MACROS_H
#define MODULE_MACROS_H

#include "../main.h"

/*
 * The macros module expands macros by replacing
 * macro invocations with their defined replacements.
 */

// Inicialitza el parser amb fitxers d'entrada i sortida
void    parser_init(FILE *input, FILE *output);

// Bucle principal del parser
void    parser_run(bool flag_c);

// Funcions que utilitza el module_comments_remove
char    parser_next_char(void);
void    parser_copy_char(char c);

#endif
