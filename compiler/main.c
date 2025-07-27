#include "ast.h"
#include "ast_analysis.h"
#include "codegen.h"
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
            "-out [output_file].s");
    // find if the input file exists
    FILE* source_file = fopen(argv[1], "r");
    if (source_file == NULL)
        panic("Could not open given file");

    TokenStream* tokenstream = lex_file(source_file);

    fclose(source_file);

    StmtList* ast = parse_tokens(tokenstream);

    if (parsing_error) {
        printf("Encountered parsing errors. Stopping compilation\n");
        free_stmt_list(ast);
    }

    free_tokenstream(tokenstream);

    // printf("Parsing sucessful... Parsing tree Representation:\n");
    AnalysisContext* context = analyse_ast(ast);

    FILE* out_file = fopen(argv[3], "w");

    int status = gen_code(context, ast, out_file);
    fclose(out_file);
    if (!status) {
        panic("Failed to generate assembly code");
    }

    return EXIT_SUCCESS;
}
