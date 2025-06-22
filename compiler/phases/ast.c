#pragma once
#include "ast.h"
#include "util.c"
#include <stdlib.h>

Expr *make_var_expr(const char *name) {}
Expr *make_const_expr(int value) {}
Expr *make_op_expr(Expr *left, Expr *right) {}
void free_expr(Expr *expr) {}

BoolExpr *make_comp_bool(Expr *left, Expr *right) {}
BoolExpr *make_not_bool(BoolExpr *child) {}
BoolExpr *make_logic_bool(BoolExpr *left, BoolExpr *right) {}
void free_bool_expr(BoolExpr *node) {}

Stmt *make_assign_stmt(const char *var_name, Expr *expr) {}
Stmt *make_if_stmt(BoolExpr *cond, Stmt *then_block, Stmt *else_block) {}
void free_stmt(Stmt *stmt) {}

StmtList *make_stmt_list() {
  StmtList *node = malloc(sizeof(StmtList));
  if (!node)
    panic("Failed to allocate memory");
  node->stmt = NULL;
  node->next = NULL; // explicit null assignment
  return node;
}

StmtList *make_stmt_list_node(Stmt *stmt) {
  StmtList *node = malloc(sizeof(StmtList));
  if (!node)
    panic("Failed to allocate memory");
  node->stmt = stmt;
  node->next = NULL; // explicit null assignment
  return node;
}

void free_stmt_list(StmtList *root);
