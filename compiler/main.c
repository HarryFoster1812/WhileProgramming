#include "./phases/lexer.h"
#include "./phases/util.c"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  // check argv
  // should be of the format [compiler_name] [input file] -out [output_file]
  if (argc != 4)
    panic("Incorrect format. Command should be [compiler_name] [input file] "
          "-out [output_file]");
  // find if the input file exists
  FILE *file_pointer = fopen(argv[1], "r");
  if (file_pointer == NULL)
    panic("Could not open given file");

  TOKEN_T **token_arr = lex_file(file_pointer);
  for (int i = 0; token_arr[i]->type != TOKEN_EOF; i++) {
    printf("Lexme: %s\n", token_arr[i]->lexeme);
    printf("type: %d\n\n", token_arr[i]->type);
  }

  return EXIT_SUCCESS;
}
