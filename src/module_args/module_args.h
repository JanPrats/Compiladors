/*
 * -----------------------------------------------------------------------------
 * module2.h 
 *
 * Header file for module2, which provides functions to handle command-line
 * arguments in a modular and testable way.
 *
 * Functions:
 * - `print_arguments`: Logs each argument received by the program.
 * - `process_arguments`: Intended to process arguments for the application.
 *
 * Usage:
 *     Include this header in main modules or test modules that require access
 *     to command-line processing functionality.
 *
 * Notes:
 *     This is part of a modular project structure, allowing each module to be
 *     developed and tested independently.
 *
 * Team: GA
 * Contributor/s: Pol García López
 * -----------------------------------------------------------------------------
 */

#ifndef MODULE2_H
#define MODULE2_H

#include "../main.h"
#include "../module_parser/module_parser.h"

ArgFlags* process_arguments(int argc, char *argv[]);
void show_help(void);
void print_arguments(int argc, char *argv[]);


#endif

