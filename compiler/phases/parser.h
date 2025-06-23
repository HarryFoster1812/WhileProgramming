#pragma once
#include "ast.h"
#include "global.c"
#include "lexer.h"
#include "tokenstream.h"
#include "util.c"

StmtList *parse_tokens(TokenStream *stream);

Stmt *parse_stmt(TokenStream *stream);
StmtList *parse_block(TokenStream *stream);
BoolExpr *parse_bool(TokenStream *stream);
Expr *parse_expr(TokenStream *stream);

#include "parser.c"
