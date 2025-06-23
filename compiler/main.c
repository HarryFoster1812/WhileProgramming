#include "./phases/lexer.h"
#include "./phases/util.c"
#include "phases/ast.h"
#include "phases/parser.c"
#include "phases/parser.h"
#include "phases/tokenstream.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    // check argv
    // should be of the format [compiler_name] [input file] -out [output_file]
    if (argc != 4)
        panic(
            "Incorrect format. Command should be [compiler_name] [input file] "
            "-out [output_file]");
    // find if the input file exists
    FILE* file_pointer = fopen(argv[1], "r");
    if (file_pointer == NULL)
        panic("Could not open given file");

    TokenStream* tokenstream = lex_file(file_pointer);

    StmtList* ast = parse_tokens(tokenstream);
    if (parsing_error) {
        printf("Encountered parsing errors. Stopping compilation");
        free_stmt_list(ast);
    }

    return EXIT_SUCCESS;
}
