#pragma once
#include "parser.h"
#include "ast.h"
#include "lexer.h"
#include "token.h"
#include "tokenstream.c"
#include "util.c"

StmtList* parse_tokens(TokenStream* stream) {
    StmtList* stmt_head = make_stmt_list();
    StmtList* stmt_current = stmt_head;
    while (!eof(stream)) {
        Stmt* parsed_stmt = parse_stmt(stream);
        if (parsed_stmt)
            stmt_current = append_stmt(stmt_current, parsed_stmt);

        if (match(stream, TOKEN_SEMICOLON)) {
            // consume semicolon
        } else if (!eof(stream)) {
            TOKEN_T* curr = peek_token(stream);
            expected_token(curr->line_number, curr->col_number, curr->type,
                           TOKEN_SEMICOLON);
        }
    }

    return stmt_head;
}

// IMPLEMENTATION:
// A top-down parser which uses recusive decent parsing
// predictive parsing for statments
// backtracking parsing fot bool and expressions

Stmt* parse_stmt(TokenStream* stream) {}
StmtList* parse_block(TokenStream* stream) {}
BoolExpr* parse_bool(TokenStream* stream) {}
Expr* parse_expr(TokenStream* stream) {}

// need to handle unexpected tokenes
