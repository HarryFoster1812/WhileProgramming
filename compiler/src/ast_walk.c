#include "ast_walk.h"
#include "ast.h"

#include "ast.h"
#include "ast_walk.h"

void walk_expr(Expr* expr, ASTCallback cb, void* ctx, TraversalOrder order) {
    if (!expr)
        return;

    if (order == TRAVERSAL_PREORDER) {
        (*cb)(expr, "expr", ctx);
    }

    switch (expr->type) {
    case EXPR_ADD:
    case EXPR_SUB:
        walk_expr(expr->op.left, cb, ctx, order);
        if (order == TRAVERSAL_INORDER)
            (*cb)(expr, "expr", ctx);
        walk_expr(expr->op.right, cb, ctx, order);
        break;

    case EXPR_CONST:
    case EXPR_VAR:
        if (order == TRAVERSAL_INORDER)
            (*cb)(expr, "expr", ctx);
        break;
    }

    if (order == TRAVERSAL_POSTORDER)
        (*cb)(expr, "expr", ctx);
}

void walk_bool_expr(BoolExpr* bexpr, ASTCallback cb, void* ctx,
                    TraversalOrder order) {
    if (!bexpr)
        return;

    if (order == TRAVERSAL_PREORDER)
        (*cb)(bexpr, "bool", ctx);

    switch (bexpr->type) {
    case BOOL_AND:
        walk_bool_expr(bexpr->logic.left, cb, ctx, order);
        if (order == TRAVERSAL_INORDER)
            (*cb)(bexpr, "bool", ctx);
        walk_bool_expr(bexpr->logic.right, cb, ctx, order);
        break;

    case BOOL_EQL:
    case BOOL_LEQ:
        walk_expr(bexpr->comp.left, cb, ctx, order);
        if (order == TRAVERSAL_INORDER)
            (*cb)(bexpr, "bool", ctx);
        walk_expr(bexpr->comp.right, cb, ctx, order);
        break;

    case BOOL_NOT:
        walk_bool_expr(bexpr->child, cb, ctx, order);
        if (order == TRAVERSAL_INORDER)
            (*cb)(bexpr, "bool", ctx);
        break;

    case BOOL_FALSE:
    case BOOL_TRUE:
        if (order == TRAVERSAL_INORDER)
            (*cb)(bexpr, "bool", ctx);
        break;
    }

    if (order == TRAVERSAL_POSTORDER)
        (*cb)(bexpr, "bool", ctx);
}

void walk_stmt(Stmt* stmt, ASTCallback cb, void* ctx, TraversalOrder order) {
    if (!stmt)
        return;

    if (order == TRAVERSAL_PREORDER)
        (*cb)(stmt, "stmt", ctx);

    switch (stmt->type) {
    case STMT_ASSIGN:
        walk_expr(stmt->assign.expr, cb, ctx, order);
        break;

    case STMT_IF:
        walk_bool_expr(stmt->if_stmt.condition, cb, ctx, order);
        walk_stmt_list(stmt->if_stmt.then_block, cb, ctx, order);
        walk_stmt_list(stmt->if_stmt.else_block, cb, ctx, order);
        break;

    case STMT_WHILE:
        walk_bool_expr(stmt->while_stmt.condition, cb, ctx, order);
        walk_stmt_list(stmt->while_stmt.body, cb, ctx, order);
        break;

    case STMT_INPUT:
    case STMT_PRINT:
    case STMT_SKIP:
        break;
    }

    if (order == TRAVERSAL_INORDER)
        (*cb)(stmt, "stmt", ctx);

    if (order == TRAVERSAL_POSTORDER)
        (*cb)(stmt, "stmt", ctx);
}

void walk_stmt_list(StmtList* list, ASTCallback cb, void* ctx,
                    TraversalOrder order) {
    while (list) {
        if (list->stmt)
            walk_stmt(list->stmt, cb, ctx, order);
        list = list->next;
    }
}
