#pragma once
#include "ast.h"

typedef void (*ASTCallback)(void *node, const char *kind, void *ctx);

void walk_expr(Expr *expr, ASTCallback cb, void *ctx);
void walk_bool_expr(BoolExpr *bexpr, ASTCallback cb, void *ctx);
void walk_stmt(Stmt *stmt, ASTCallback cb, void *ctx);
void walk_stmt_list(StmtList *list, ASTCallback cb, void *ctx);
