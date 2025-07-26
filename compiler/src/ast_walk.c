#include "ast_walk.h"
#include "ast.h"

void walk_expr(Expr* expr, ASTCallback cb, void* ctx) {

    (*cb)(expr, "expr", ctx);

    switch (expr->type) {
    case EXPR_ADD:
    case EXPR_SUB:
        walk_expr(expr->op.left, cb, ctx);
        walk_expr(expr->op.right, cb, ctx);
        break;

    case EXPR_CONST:
    case EXPR_VAR:
        // No children to walk
        break;
    }
}

void walk_bool_expr(BoolExpr* bexpr, ASTCallback cb, void* ctx) {

    (*cb)(bexpr, "bool", ctx);

    switch (bexpr->type) {
    case BOOL_AND:
        walk_bool_expr(bexpr->logic.left, cb, ctx);
        walk_bool_expr(bexpr->logic.right, cb, ctx);
        break;

    case BOOL_EQL:
    case BOOL_LEQ:
        walk_expr(bexpr->comp.left, cb, ctx);
        walk_expr(bexpr->comp.right, cb, ctx);
        break;

    case BOOL_NOT:
        walk_bool_expr(bexpr->child, cb, ctx);
        break;

    case BOOL_FALSE:
    case BOOL_TRUE:
        break;
    }
}

void walk_stmt(Stmt* stmt, ASTCallback cb, void* ctx) {

    (*cb)(stmt, "stmt", ctx);

    switch (stmt->type) {
    case STMT_ASSIGN:
        walk_expr(stmt->assign.expr, cb, ctx);
        break;

    case STMT_IF:
        walk_bool_expr(stmt->if_stmt.condition, cb, ctx);
        walk_stmt_list(stmt->if_stmt.then_block, cb, ctx);
        walk_stmt_list(stmt->if_stmt.else_block, cb, ctx);
        break;

    case STMT_WHILE:
        walk_bool_expr(stmt->while_stmt.condition, cb, ctx);
        walk_stmt_list(stmt->while_stmt.body, cb, ctx);
        break;

    case STMT_INPUT:
    case STMT_PRINT:
    case STMT_SKIP:
        break;
    }
}

void walk_stmt_list(StmtList* list, ASTCallback cb, void* ctx) {
    while (list) {
        if (list->stmt)
            walk_stmt(list->stmt, cb, ctx);
        list = list->next;
    }
}
