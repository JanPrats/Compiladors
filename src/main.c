#include "./main.h"
#include "./module_parser/parser.h"

FILE* ofile = NULL; // The output handler for the project run

int main(int argc, char *argv[]) {
    int n = 0;

    errors_init();

    ofile = stdout; // Default output to stdout

    fprintf(ofile, "Starting module args ...\n");
    
    ArgFlags* flags = process_arguments(argc, argv);
    if (!flags) { //Something wesnt wrong since module_args did not return the flags
        return 1;
    }

    // Show help if requested
    if (flags->show_help) {
        show_help();
        free(flags);
        return 0;
    }

    fprintf(stdout, "Input file: %s\n", flags->input_file);
    fprintf(stdout, "Output file: %s\n", flags->output_file);
    fprintf(stdout, "Flags: remove_comments=%d, process_directives=%d\n",
            flags->remove_comments, flags->process_directives);

    errors_finalize();

    if (errors_count() > 0) {
        return 1; // Devuelve error al sistema
    // Initialize parser
    ParserState* state = init_parser(flags->input_file, flags->output_file, flags);
    if (!state) {
        free(flags);
        return 1;
    }

    fprintf(stdout, "Preprocessing...\n");

    // Parse the input file until EOF
    // We use an empty array (only NULL) so parse_until will return -1 when it reaches actual EOF
    const char* no_stop[] = {NULL};  // Empty array means parse until actual EOF
    int result = parse_until(state, no_stop, true);

    // Cleanup
    cleanup_parser(state);
    free(flags);

    if (result == -1) {
        // -1 means we reached EOF, which is expected and successful
        fprintf(stdout, "Preprocessing completed successfully!\n");
        fprintf(stdout, "Output written to: %s\n", flags->output_file);
    } else {
        fprintf(stderr, "Preprocessing failed with errors.\n");
        return 1;
    }

    return 0;
}