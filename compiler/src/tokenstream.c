#include "tokenstream.h"
#include "util.h"
#include <stdlib.h>

// return the current token
TOKEN_T* peek_token(TokenStream* stream) {
    return stream->tokens[stream->index];
}

// move to the next token
TOKEN_T* next_token(TokenStream* stream) {
    TOKEN_T* current_token = peek_token(stream);
    if (current_token == NULL)
        return NULL;   // if current token is NULL, end of stream
    (stream->index)++; // move to next position
    return current_token;
}

// returns if the token exists and it matches the given type
int match(TokenStream* stream, enum TOKEN_TYPE type) {
    TOKEN_T* current_token = peek_token(stream);
    if (current_token->type == type) {
        // consume the token and return true
        next_token(stream);
        return 1;
    }
    return 0;
}

// consumes and returns the token, outputs an error otherwise
TOKEN_T* expect(TokenStream* stream, enum TOKEN_TYPE type) {
    TOKEN_T* current_token = peek_token(stream);
    if (current_token->type != type) {
        expected_token(current_token->line_number, current_token->col_number,
                       current_token->type, type);
        return peek_token(stream); // do not advance the token
    }
    return next_token(stream);
}

int peek_token_is(TokenStream* stream, enum TOKEN_TYPE type) {
    TOKEN_T* current_token = peek_token(stream);
    if (current_token->type == type)
        return 1;
    return 0;
}

int eof(TokenStream* stream) {
    TOKEN_T* current_token = peek_token(stream);
    if (current_token->type == TOKEN_EOF)
        return 1;
    return 0;
}

void seek_next_terminal(TokenStream* stream) {
    const int terminal_symbols[] = {TOKEN_SEMICOLON, TOKEN_L_CURLY,
                                    TOKEN_R_CURLY};
    int terminal_symbols_length = sizeof(terminal_symbols) / sizeof(int);

    while (1) {
        int match_symbol = 0;

        for (int i = 0; i < terminal_symbols_length; ++i) {
            if (match(stream, terminal_symbols[i])) {
                match_symbol = 1;
                break; // No need to check further, we found a terminal
            }
        }

        if (match_symbol) {
            break;
        } else {
            next_token(stream);
        }
    }
}

void free_tokenstream(TokenStream* stream) {
    for (int i = 0; i < stream->length; ++i) {
        free_token(stream->tokens[i]);
    }
    free(stream);
}
