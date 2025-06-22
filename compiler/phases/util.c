#pragma once
#include <stdio.h>
#include <stdlib.h>

void panic(const char *message) {
  fprintf(stderr, "%s.\n", message);
  exit(1);
}

void expected_token(int line, int column, const char *token) {
  fprintf(stderr, "ERROR: on line %d, column %d. Expected token: %s.\n", line,
          column, token);
  exit(1);
}

void unexpected_token(int line, int column, const char token) {
  fprintf(stderr,
          "ERROR: on line %d, column %d. Unexpected token whilst lexing: %c.\n",
          line, column, token);
  exit(1);
}
