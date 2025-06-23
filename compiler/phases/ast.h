#pragma once

typedef enum {
  EXPR_VAR,
  EXPR_CONST,
  EXPR_ADD,
  EXPR_SUB,
} ExprType;

typedef enum {
  BOOL_TRUE,
  BOOL_FALSE,
  BOOL_AND,
  BOOL_EQL,
  BOOL_LEQ,
  BOOL_NOT

} BoolExprType;

typedef enum {
  STMT_ASSIGN,
  STMT_SKIP,
  STMT_IF,
  STMT_WHILE,
  STMT_PRINT,
  STMT_INPUT
} StmtType;

typedef struct NodeExpr Expr;
typedef struct NodeBoolExpr BoolExpr;
typedef struct NodeStmtExpr Stmt;

typedef struct NodeExpr {
  ExprType type;
  union {
    char *var_name; // for EXPR_VAR
    int constant;   // for EXPR_CONST
    struct {        // for binary ops
      struct NodeExpr *left;
      struct NodeExpr *right;
    } op;
  };
} Expr;

typedef struct NodeBoolExpr {
  BoolExprType type;
  union {

    struct {
      struct NodeExpr *left;
      struct NodeExpr *right;
    } comp;

    struct NodeBoolExpr *child; // For NOT

    struct {
      struct NodeBoolExpr *left;
      struct NodeBoolExpr *right;
    } logic; // AND
  };
} BoolExpr;

typedef struct StmtList {
  Stmt *stmt;
  struct StmtList *next;
} StmtList;

typedef struct NodeStmtExpr {
  StmtType type;
  union {
    struct {
      char *var_name;
      struct NodeExpr *expr;
    } assign;

    struct {
      struct NodeBoolExpr *condition;
      struct StmtList *then_block;
      struct StmtList *else_block;
    } if_stmt;

    struct {
      struct NodeBoolExpr *condition;
      struct StmtList *body;
    } while_stmt;

    struct {
      char *var_name;
    } print_input;
  };
} Stmt;

Expr *make_var_expr(const char *name);
Expr *make_const_expr(int value);
Expr *make_op_expr(ExprType type, Expr *left, Expr *right);
void free_expr(Expr *expr);

BoolExpr *make_comp_bool(BoolExprType opType, Expr *left, Expr *right);
BoolExpr *make_tf_bool(BoolExprType boolType);
BoolExpr *make_not_bool(BoolExpr *child);
BoolExpr *
make_logic_bool(BoolExprType type, BoolExpr *left,
                BoolExpr *right); // even though there is only && we pass type
                                  // so that it can be extended
void free_bool_expr(BoolExpr *node);

Stmt *make_assign_stmt(const char *var_name, Expr *expr);
Stmt *make_if_stmt(BoolExpr *cond, StmtList *then_block, StmtList *else_block);
Stmt *make_while_stmt(BoolExpr *cond, StmtList *block);
// ONLY IO OPERATIONS ARE INPUT AND OUTPUT
Stmt *make_io_stmt(StmtType type, const char *var_name);
void free_stmt(Stmt *stmt);

StmtList *make_stmt_list();
StmtList *make_stmt_list_node(Stmt *stmt);
void free_stmt_list(StmtList *root);

#include "ast.c"
