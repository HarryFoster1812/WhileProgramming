#pragma once
#include "global.c"
#include "token.h"
#include <stdio.h>
#include <stdlib.h>

void panic(const char* message) {
    fprintf(stderr, "%s.\n", message);
    exit(1);
}

const char* token_type_str(enum TOKEN_TYPE token) {
    return TOKEN_TYPE_MAP[token];
}

void expected_token(int line, int column, enum TOKEN_TYPE recived,
                    enum TOKEN_TYPE expected) {
    fprintf(stderr,
            "ERROR: on line %d, column %d. Recived token type %s but expected "
            "token type %s.\n",
            line, column, token_type_str(recived), token_type_str(expected));
    parsing_error = 1;
}

void unexpected_token(int line, int column, const char* token) {
    fprintf(stderr, "ERROR: on line %d, column %d. Unexpected token %s.\n",
            line, column, token);
    parsing_error = 1;
}

void expected_comp(int line, int column, const char* token) {
    fprintf(stderr,
            "ERROR: on line %d, column %d. Unexpected token %s. Expected a "
            "comparison operator such as == or <=\n",
            line, column, token);
    parsing_error = 1;
}

void failed_parse() { fprintf(stderr, "Failed to parse statement"); }
