#ifndef MODULE_ERRORS_H
#define MODULE_ERRORS_H

#include "../main.h"

/*
 * The errors module detects and reports preprocessing
 * errors and warnings encountered during preprocessing.
 */

#include <stdio.h>

/* Error severity levels */
typedef enum {
    ERROR_WARNING,
    ERROR_ERROR
} ErrorLevel;

/* Initializes the error system */
void errors_init(void);

/* Report an error or warning */
void report_error(
    ErrorLevel level,
    const char *filename,
    int line,
    const char *message
);

/* Returns the number of errors detected */
int errors_count(void);

/* Called at the end of preprocessing */
void errors_finalize(void);

#endif

