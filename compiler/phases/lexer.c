#pragma once
#include "./lexer.h"
#include "tokenstream.h"
#include "util.c"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum TOKEN_TYPE identify_token(const char* lexeme) {

    const int keyword_count = sizeof(keywords) / sizeof(TokenMap);
    for (int i = 0; i < keyword_count; ++i) {
        if (strcmp(lexeme, keywords[i].lexeme) == 0) {
            return keywords[i].type;
        }
    }

    if (isdigit(lexeme[0]))
        return TOKEN_NUMBER;
    if (isalpha(lexeme[0]))
        return TOKEN_VARIABLE;
    return TOKEN_UNKNOWN;
}

TokenStream* lex_file(FILE* fptr) {
    // file buffer
    TokenStream* tokenstream = (TokenStream*)malloc(sizeof(TokenStream));
    long token_count = 0;
    long token_arr_size = 2000;
    int file_buffer_size = 2048;
    TOKEN_T** token_arr = (TOKEN_T**)malloc(sizeof(TOKEN_T*) * token_arr_size);
    char fbuffer[file_buffer_size];
    int line_no = 1;
    while (fgets(fbuffer, file_buffer_size, fptr)) {
        // for each line in the file
        int col_no = 0;
        while (fbuffer[col_no] != '\0') {

            while (isspace(fbuffer[col_no]))
                col_no++;

            if (fbuffer[col_no] == '\n' || fbuffer[col_no] == '\0')
                break;

            // we are now at the start of a token

            int col_start = col_no; // this is used to index tokens
                                    //
            if (isalpha(fbuffer[col_no])) {
                // Identifier or keyword
                while (isalnum(fbuffer[col_no]))
                    col_no++;
            } else if (isdigit(fbuffer[col_no])) {
                // Number
                while (isdigit(fbuffer[col_no]))
                    col_no++;
            } else if (is_symbol(fbuffer[col_no])) {
                // Try to match multi-character operators first
                if (is_multi_character_symbol(fbuffer, &col_no)) {

                } else {
                    col_no++; // Single-character symbol
                }
            } else {
                // Unknown character, treat as single unknown token
                col_no++;
            }

            char* tok = (char*)malloc(col_no - col_start + 1);
            // +2 since we also need to account for the null character

            for (int i = 0; i < col_no - col_start; ++i) {
                tok[i] = fbuffer[col_start + i];
            }
            // null terminate the string for the token
            tok[col_no - col_start + 1] = 0;

            enum TOKEN_TYPE type = identify_token(tok);
            // add tokens into the arr

            if (token_count == token_arr_size) {
                // extend token_arr to fit the new values
                token_arr_size *= 2;
                TOKEN_T** new_arr = (TOKEN_T**)realloc(
                    token_arr, sizeof(TOKEN_T*) * token_arr_size);
                if (!new_arr) {
                    panic("Memory Realloc failed");
                }
                token_arr = new_arr;
            }

            token_arr[token_count++] =
                make_token(type, line_no, col_start, tok);
        }
        line_no++;
    }
    token_arr[token_count++] = make_token(TOKEN_EOF, line_no, 0, "EOF");
    // realloc to free any unused Memory
    token_arr = realloc(token_arr, sizeof(TOKEN_T*) * token_count);

    tokenstream->tokens = token_arr;
    tokenstream->length = token_count;
    tokenstream->index = 0;

    return tokenstream;
}
