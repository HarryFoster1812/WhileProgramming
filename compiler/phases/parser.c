#pragma once
#include "parser.h"
#include "util.c"

Stmt *parse_tokens(TokenStream stream) { TokenStream *position = &stream; }

// IMPLEMENTATION:
// A top-down parser which uses recusive decent parsing
// predictive parsing for statments
// backtracking parsing fot bool and expressions

Stmt *parse_stmt(TokenStream *stream) {}
BoolExpr *parse_bool(TokenStream *stream) {}
Expr *parse_expr(TokenStream *stream) {}

// need to handle unexpected tokenes
