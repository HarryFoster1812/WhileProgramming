#include "ast.h"
#include "ast_analysis.h"
#include "lexer.h"
#include "parser.h"
#include "symbol_table.h"
#include "tokenstream.h"
#include "util.h"
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
        printf("Encountered parsing errors. Stopping compilation\n");
        free_stmt_list(ast);
    }

    free_tokenstream(tokenstream);

    printf("Parsing sucessful... Parsing tree Representation:\n");
    AnalysisContext* context = analyse_ast(ast);
    print_symbol_table(context->symtab);
    printf("Uses input %d\n", context->uses_input);
    printf("Uses print %d\n", context->uses_print);

    return EXIT_SUCCESS;
}
