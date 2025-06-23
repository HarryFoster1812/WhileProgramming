#pragma once
#include "parser.h"
#include "ast.h"
#include "token.h"
#include "tokenstream.c"
#include "util.c"
#include <math.h>

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

Stmt* parse_stmt(TokenStream* stream) {
    TOKEN_T* current = peek_token(stream);
    if (match(stream, TOKEN_VARIABLE)) {
        expect(stream, TOKEN_ASSIGNMENT);
        Expr* rhs = parse_expr(stream);
        return make_assign_stmt(current->lexeme, rhs);

    } else if (match(stream, TOKEN_IF)) {

        expect(stream, TOKEN_L_PAREN);
        BoolExpr* cond = parse_bool(stream);
        expect(stream, TOKEN_R_PAREN);

        expect(stream, TOKEN_THEN);

        expect(stream, TOKEN_L_CURLY);
        StmtList* then_block = parse_block(stream);
        expect(stream, TOKEN_R_CURLY);

        expect(stream, TOKEN_ELSE);

        expect(stream, TOKEN_L_CURLY);
        StmtList* else_block = parse_block(stream);
        expect(stream, TOKEN_R_CURLY);

        return make_if_stmt(cond, then_block, else_block);

    } else if (match(stream, TOKEN_WHILE)) {
        expect(stream, TOKEN_L_PAREN);
        BoolExpr* cond = parse_bool(stream);
        expect(stream, TOKEN_R_PAREN);

        expect(stream, TOKEN_DO);

        expect(stream, TOKEN_L_CURLY);
        StmtList* inner_block = parse_block(stream);
        expect(stream, TOKEN_R_CURLY);

        return make_while_stmt(cond, inner_block);
    } else if (match(stream, TOKEN_INPUT)) {
        TOKEN_T* var = expect(stream, TOKEN_VARIABLE);
        if (var)
            return make_io_stmt(STMT_INPUT, var->lexeme);
        return make_io_stmt(STMT_INPUT, "");
    } else if (match(stream, TOKEN_WRITE)) {
        TOKEN_T* var = expect(stream, TOKEN_VARIABLE);
        if (var)
            return make_io_stmt(STMT_PRINT, var->lexeme);
        return make_io_stmt(STMT_PRINT, "");
    } else if (match(stream, TOKEN_SKIP)) {
        return make_skip_stmt();
    } else {
        unexpected_token(current->line_number, current->col_number,
                         current->lexeme);
    }
}
StmtList* parse_block(TokenStream* stream) {}
BoolExpr* parse_bool(TokenStream* stream) {}
Expr* parse_expr(TokenStream* stream) {}

// need to handle unexpected tokenes
