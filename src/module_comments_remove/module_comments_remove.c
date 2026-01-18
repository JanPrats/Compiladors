#include <stdio.h>
#include "./module_comments_remove.h"
#include "../module_parser/parser.h"


void module_comments_run(void) {
    printf("Loaded module_comments: comments removal module\n");
}

#include <stdbool.h>

/*
==========================================================
 FUNCIONS QUE EL PARSER PROPORCIONA
==========================================================
El mòdul de comentaris NO sap res del fitxer.
Només consumeix caràcters via el parser.
*/
char    parser_next_char(void);
void    parser_copy_char(char c);

/*
==========================================================
 Ignora un comentari de línia //
==========================================================
Consumeix caràcters fins trobar '\n'
*/
static void skip_line_comment(void)
{
    char c;

    while ((c = parser_next_char()) != '\n')
        ;
}

/*
==========================================================
 Copia un comentari de línia //
==========================================================
Copia tot fins '\n'
*/
static void copy_line_comment(void)
{
    char c;

    // Ja tenim "//"
    parser_copy_char('/');
    parser_copy_char('/');

    while ((c = parser_next_char()) != '\n')
        parser_copy_char(c);

    parser_copy_char('\n');
}

/*
==========================================================
 Ignora un comentari de bloc /* ... * /
==========================================================
Consumeix fins detectar */

static void skip_block_comment(void)
{
    char c;
    char prev = 0;

    while (1)
    {
        c = parser_next_char();
        if (prev == '*' && c == '/')
            break;
        prev = c;
    }
}

/*
==========================================================
 Copia un comentari de bloc /* ... * /
==========================================================
Copia fins detectar */
static void copy_block_comment(void)
{
    char c;
    char prev = 0;

    // Ja tenim "/*"
    parser_copy_char('/');
    parser_copy_char('*');

    while (1)
    {
        c = parser_next_char();
        parser_copy_char(c);

        if (prev == '*' && c == '/')
            break;

        prev = c;
    }
}

/*
==========================================================
 comment_handle
==========================================================
FUNCIO PRINCIPAL DEL MÒDUL

PARAMETRES:
- first : sempre serà '/'
- flag_c: indica si cal copiar comentaris

RETORN:
- true  -> era un comentari
- false -> NO era un comentari
*/
bool comment_handle(char first, bool flag_c)
{
    char next;

    (void)first;

    // Llegim el següent caràcter
    next = parser_next_char();

    // CAS //
    if (next == '/')
    {
        if (flag_c)
            copy_line_comment();
        else
            skip_line_comment();
        return true;
    }

    // CAS /* */
    if (next == '*')
    {
        if (flag_c)
            copy_block_comment();
        else
            skip_block_comment();
        return true;
    }

    /*
    No era un comentari.
    El parser haurà de gestionar '/' i next com a codi normal.
    */
    return false;
}
