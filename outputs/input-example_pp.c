/*
 * Compilers
 * Input example code: Define simple .c and .h files to test your processing
 * File name: input-example.c
 * Additional files: input-example.h
 */

/*
 * Compilers
 * Input example code: Define simple .c and .h files to test your processing
 * File name: input-example.h
 * Additional files: input-example.c
 */
// Just comment them out or leave the include untouched.
//#include <stdio.h>
//#include <stdlib.h>     // To set env for time zone
//#include <string.h>     // For filename manipulation   




// MACROS used for the printouts instead of using f/printfs so that all prints
// in the program are treated the same way.
// Use ERROR when the print out informs of a ERROR/problem in the program and it must finish the program
// Use WARNING when the print out informs of a ERROR/problem in the program but the preprocessor can continue
// You have to define the format of your errors, this is just an example to test, 
// but your code can do a different version of this to treat errors
/* usage: ERROR(("Warning: Note the two brackets\n")) */
                        printf("ERROOORRRRRRRRRR see output file\n");})

FILE    *errfile;        // File where to write error messages
FILE    *ofile;          // File where to write program information



int main(int argc, char**argv) {
    
    int i;
 
    //printf("I am in main function \n");
    ofile = stdout;             // default is stdout
    ofile = fopen( "out.txt"    // file destination of normal program print: stdout/out.txt, "w");
    if(ofile == NULL)                 
        (num, message)({fprintf(errfile,"%d ERROR: ", num);fprintf message;\(0, (ofile, "Problem creating %s\n", "out.txt"    // file destination of normal program print: stdout/out.txt));

    errfile = stdout;           // default is stdout
    errfile = fopen( "errmsg.txt"   // file destination of normal program print: errmsg.txt, "w");
    if(errfile != NULL)                
        (num, message)({fprintf(errfile,"%d ERROR: ", num);fprintf message;\(0, (ofile, "Problem creating %s\n", "errmsg.txt"   // file destination of normal program print: errmsg.txt));
    

    // Prints arguments
    fprintf(ofile, "Arguments:\n");
    for (i = 0; i < argc; i++) {
        fprintf(ofile, "%i: %s\n", i, argv[i]);
(ON     // ON = Yes OFF / NO == 1)
        (num, message)({fprintf(errfile,"%d ERROR: ", num);fprintf message;\(i, (errfile, "warning print example %s \n", argv[i]));
    }
    
 (ON     // ON = Yes OFF / NO == 1)
    (num, message)({WARNING( num, message); exit(0);})(i, (errfile, "This is an error, and it stops"));
    
    fclose(ofile);
    fclose(errfile);
    
    return 0;
}
