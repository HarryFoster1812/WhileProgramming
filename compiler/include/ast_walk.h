#pragma once
#include "ast.h"
typedef enum {
  TRAVERSAL_PREORDER,
  TRAVERSAL_INORDER,
  TRAVERSAL_POSTORDER
} TraversalOrder;

typedef void (*ASTCallback)(void *node, const char *kind, void *ctx);

void walk_expr(Expr *expr, ASTCallback cb, void *ctx, TraversalOrder order,
               int walk_child_blocks);
void walk_bool_expr(BoolExpr *bexpr, ASTCallback cb, void *ctx,
                    TraversalOrder order, int walk_child_blocks);
void walk_stmt(Stmt *stmt, ASTCallback cb, void *ctx, TraversalOrder order,
               int walk_child_blocks);
void walk_stmt_list(StmtList *list, ASTCallback cb, void *ctx,
                    TraversalOrder order, int walk_child_blocks);
