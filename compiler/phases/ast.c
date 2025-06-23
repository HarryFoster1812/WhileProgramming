#pragma once
#include "ast.h"
#include "util.c"
#include <stdlib.h>
#include <string.h>

// DEV NOTE:
// I AM TERRIBLE AT C.
// IT IS ALMOST GUARENTEED THAT THERE IS A MEMORY LEAK SOMEWHERE HERE
// IF THERE ISNT THEN I AM AMAZING AT C.

Expr *make_var_expr(const char *name) {
  Expr *astNode = calloc(1, sizeof(Expr));
  if (astNode == NULL)
    panic("Failed to allocate memory");

  astNode->type = EXPR_VAR;
  astNode->var_name = calloc(strlen(name) + 1, sizeof(char));
  if (astNode->var_name == NULL)
    panic("Failed to allocate memory");

  strcpy(astNode->var_name, name);
  return astNode;
}

Expr *make_const_expr(int value) {

  Expr *astNode = calloc(1, sizeof(Expr));
  if (astNode == NULL)
    panic("Failed to allocate memory");

  astNode->type = EXPR_CONST;
  astNode->constant = value;
  return astNode;
}
Expr *make_op_expr(ExprType type, Expr *left, Expr *right) {
  Expr *astNode = calloc(1, sizeof(Expr));
  if (astNode == NULL)
    panic("Failed to allocate memory");

  astNode->type = type; // this should only be EXPR_ADD ot EXPR_SUB (or similar
                        // if added later)
  astNode->op.left = left;
  astNode->op.right = right;
  return astNode;
}

// I am NOT great at C. I hope this doesnt cause memory leaks...
void free_expr(Expr *expr) {
  if (expr == NULL) {
    panic("Provided Expression to free was NULL");
  }
  if (expr->type == EXPR_VAR) {
    free(expr->var_name);
  } else if (expr->type == EXPR_ADD || expr->type == EXPR_SUB) {
    free_expr(expr->op.left);
    free_expr(expr->op.right);
  }
  free(expr);
}

BoolExpr *make_comp_bool(BoolExprType opType, Expr *left, Expr *right) {
  BoolExpr *astNode = calloc(1, sizeof(BoolExpr));
  if (astNode == NULL)
    panic("Failed to allocate memory");

  astNode->type =
      opType; // this should only be BOOLEXPR_EQL or BOOLEXPR_LEQ ect..
  astNode->comp.left = left;
  astNode->comp.right = right;
  return astNode;
}

BoolExpr *make_not_bool(BoolExpr *child) {
  BoolExpr *astNode = calloc(1, sizeof(BoolExpr));
  if (astNode == NULL)
    panic("Failed to allocate memory");

  astNode->type = BOOL_NOT;
  // similar if added later)
  astNode->child = child;
  return astNode;
}
BoolExpr *make_logic_bool(BoolExprType type, BoolExpr *left, BoolExpr *right) {
  BoolExpr *astNode = calloc(1, sizeof(BoolExpr));
  if (astNode == NULL)
    panic("Failed to allocate memory");

  astNode->type = type; // this should only be BOOL_AND
  astNode->logic.left = left;
  astNode->logic.right = right;
  return astNode;
}

BoolExpr *make_tf_bool(BoolExprType boolType) {
  BoolExpr *astNode = calloc(1, sizeof(BoolExpr));
  if (astNode == NULL)
    panic("Failed to allocate memory");

  astNode->type = boolType; // This should be BOOL_TRUE, BOOL_FALSE
  return astNode;
}

void free_bool_expr(BoolExpr *node) {
  if (node == NULL)
    panic("Provided BoolExpr is NULL");

  if (node->type == BOOL_NOT) {
    free_bool_expr(node->child);

  } else if (node->type == BOOL_LEQ || node->type == BOOL_EQL) {
    free_expr(node->comp.left);
    free_expr(node->comp.right);

  } else if (node->type == BOOL_AND) {
    free_bool_expr(node->logic.left);
    free_bool_expr(node->logic.right);
  }

  free(node);
}

Stmt *make_assign_stmt(const char *var_name, Expr *expr) {
  Stmt *astNode = calloc(1, sizeof(Stmt));
  if (astNode == NULL)
    panic("Failed to allocate memory");

  astNode->type = STMT_ASSIGN;
  astNode->assign.expr = expr;
  astNode->assign.var_name = calloc(strlen(var_name) + 1, sizeof(char));
  strcpy(astNode->assign.var_name, var_name);
  return astNode;
}

Stmt *make_if_stmt(BoolExpr *cond, StmtList *then_block, StmtList *else_block) {
  Stmt *astNode = calloc(1, sizeof(Stmt));
  if (astNode == NULL)
    panic("Failed to allocate memory");

  astNode->type = STMT_IF;
  astNode->if_stmt.condition = cond;
  astNode->if_stmt.then_block = then_block;
  astNode->if_stmt.else_block = else_block;
  return astNode;
}

Stmt *make_while_stmt(BoolExpr *cond, StmtList *block) {
  Stmt *astNode = calloc(1, sizeof(Stmt));
  if (astNode == NULL)
    panic("Failed to allocate memory");

  astNode->type = STMT_WHILE;
  astNode->while_stmt.condition = cond;
  astNode->while_stmt.body = block;
  return astNode;
}

Stmt *make_io_stmt(StmtType type, const char *var_name) {
  Stmt *astNode = calloc(1, sizeof(Stmt));
  if (astNode == NULL)
    panic("Failed to allocate memory");

  astNode->type = type;
  astNode->print_input.var_name = calloc(strlen(var_name) + 1, sizeof(char));
  strcpy(astNode->print_input.var_name, var_name);
  return astNode;
}

void free_stmt(Stmt *stmt) {
  if (stmt == NULL)
    panic("Provided Statement is NULL");

  if (stmt->type == STMT_ASSIGN) {
    free(stmt->assign.var_name);
    free_expr(stmt->assign.expr);

  } else if (stmt->type == STMT_WHILE) {
    free_bool_expr(stmt->while_stmt.condition);
    free_stmt_list(stmt->while_stmt.body);

  } else if (stmt->type == STMT_IF) {
    free_bool_expr(stmt->if_stmt.condition);
    free_stmt_list(stmt->if_stmt.then_block);
    free_stmt_list(stmt->if_stmt.else_block);

  } else if (stmt->type == STMT_PRINT || stmt->type == STMT_INPUT) {
    free(stmt->print_input.var_name);
  }

  free(stmt);
}

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

void free_stmt_list(StmtList *root) {
  while (root != NULL) {
    StmtList *current = root->next;
    if (root->stmt)
      free_stmt(root->stmt);
    free(root);
    root = current;
  }
}
