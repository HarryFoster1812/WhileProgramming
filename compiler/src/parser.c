#include "parser.h"
#include "util.h"
#include <stdlib.h>

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
            seek_next_terminal(stream);
        }
    }

    return stmt_head;
}

StmtList* parse_block(TokenStream* stream) {
    StmtList* stmt_head = make_stmt_list();
    StmtList* stmt_current = stmt_head;

    while (!eof(stream) && !peek_token_is(stream, TOKEN_R_CURLY)) {
        Stmt* parsed_stmt = parse_stmt(stream);
        if (parsed_stmt)
            stmt_current = append_stmt(stmt_current, parsed_stmt);

        if (match(stream, TOKEN_SEMICOLON)) {
            // consume semicolon
        } else if (!eof(stream) && !peek_token_is(stream, TOKEN_R_CURLY)) {
            TOKEN_T* curr = peek_token(stream);
            expected_token(curr->line_number, curr->col_number, curr->type,
                           TOKEN_SEMICOLON);
            // seek to the next statment or semi colon
            seek_next_terminal(stream);
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

    } else if (match(stream, TOKEN_PRINT)) {
        TOKEN_T* var = expect(stream, TOKEN_VARIABLE);
        if (var)
            return make_io_stmt(STMT_PRINT, var->lexeme);
        return make_io_stmt(STMT_PRINT, "");

    } else if (match(stream, TOKEN_SKIP)) {
        return make_skip_stmt();

    } else {
        unexpected_token(current->line_number, current->col_number,
                         current->lexeme);
        return NULL;
    }
}

BoolExpr* parse_comp_bool(TokenStream* stream) {
    int before_parse_expr = stream->index;
    int parse_status_before = parsing_error;
    Expr* lexpr = parse_expr(stream);
    TOKEN_T* current = peek_token(stream);

    if (match(stream, TOKEN_LEQ)) {
        Expr* rexpr = parse_expr(stream);
        return make_comp_bool(BOOL_LEQ, lexpr, rexpr);

    } else if (match(stream, TOKEN_EQUAL)) {
        Expr* rexpr = parse_expr(stream);
        return make_comp_bool(BOOL_EQL, lexpr, rexpr);

    } else {
        TOKEN_T* tok = peek_token(stream);
        unexpected_token(tok->line_number, tok->col_number, tok->lexeme);
        return NULL;
    }
}

BoolExpr* parse_bool_not(TokenStream* stream) {
    if (match(stream, TOKEN_NOT)) {
        BoolExpr* sub = parse_bool_not(stream);
        return make_not_bool(sub);

    } else {
        return parse_bool_primary(stream);
    }
}

BoolExpr* parse_bool_primary(TokenStream* stream) {
    if (match(stream, TOKEN_TRUE)) {
        return make_tf_bool(BOOL_TRUE);

    } else if (match(stream, TOKEN_FALSE)) {
        return make_tf_bool(BOOL_FALSE);

    } else if (match(stream, TOKEN_L_PAREN)) {
        BoolExpr* expr = parse_bool(stream);
        expect(stream, TOKEN_R_PAREN);
        return expr;

    } else {
        return parse_comp_bool(stream);
    }
}

// This is the top level
BoolExpr* parse_bool(TokenStream* stream) {
    // top level parse comp
    BoolExpr* lcomp = parse_bool_not(stream);
    while (match(stream, TOKEN_AND)) {
        BoolExpr* rcomp = parse_bool(stream);
        lcomp = make_logic_bool(BOOL_AND, lcomp, rcomp);
    }
    return lcomp;
}

Expr* parse_factor(TokenStream* stream) {
    TOKEN_T* current = peek_token(stream);
    if (match(stream, TOKEN_NUMBER)) {
        return make_const_expr(atoi(current->lexeme)); // e.g. 2, 9, etc.
    } else if (match(stream, TOKEN_VARIABLE)) {
        return make_var_expr(current->lexeme);
    } else {
        TOKEN_T* tok = peek_token(stream);
        unexpected_token(tok->line_number, tok->col_number, tok->lexeme);
        return NULL;
    }
}

Expr* parse_term(TokenStream* stream) {
    if (match(stream, TOKEN_L_PAREN)) {
        Expr* inner = parse_expr(stream);
        expect(stream, TOKEN_R_PAREN);
        return inner;
    } else {
        return parse_factor(stream);
    }
}

Expr* parse_expr(TokenStream* stream) {
    Expr* left = parse_term(stream);
    while (1) {
        if (match(stream, TOKEN_PLUS)) {
            Expr* right = parse_term(stream);
            left = make_op_expr(EXPR_ADD, left, right);
        } else if (match(stream, TOKEN_MINUS)) {
            Expr* right = parse_term(stream);
            left = make_op_expr(EXPR_SUB, left, right);
        } else {
            break;
        }
    }
    return left;
}

// need to handle unexpected tokenes
