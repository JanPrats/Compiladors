#include <stdio.h>
#include <stdbool.h>
#include "./module_parser.h"
#include "../module_comments_remove/comments_remove.h"


/*
==========================================================
 VARIABLES GLOBALS DEL PARSER
==========================================================
*/
static FILE *g_input_file = NULL;
static FILE *g_output_file = NULL;

/*
==========================================================
 parser_init
==========================================================
*/
void parser_init(FILE *input, FILE *output)
{
    g_input_file = input;
    g_output_file = output;
}

/*
==========================================================
 parser_next_char
==========================================================
*/
char parser_next_char(void)
{
    int c;

    if (!g_input_file)
        return EOF;

    c = fgetc(g_input_file);
    return (char)c;
}

/*
==========================================================
 parser_copy_char
==========================================================
*/
void parser_copy_char(char c)
{
    if (g_output_file)
        fputc(c, g_output_file);
}

/*
==========================================================
 parser_run
==========================================================
*/
void parser_run(bool flag_c)
{
    char c;

    while ((c = parser_next_char()) != EOF)
    {
        if (c == '/')
        {
            /*
            Deleguem al mòdul de comentaris
            */
            if (comment_handle(c, flag_c))
                continue;

            // No era un comentari
            parser_copy_char(c);
        }
        else
        {
            parser_copy_char(c);
        }
    }
}
