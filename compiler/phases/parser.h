#pragma once
#include "ast.h"
#include "lexer.h"

typedef TOKEN_T **TokenStream;

Stmt *parse_tokens(TokenStream stream);

Stmt *parse_stmt(TokenStream *stream);
BoolExpr *parse_bool(TokenStream *stream);
Expr *parse_expr(TokenStream *stream);

#include "parser.c"
