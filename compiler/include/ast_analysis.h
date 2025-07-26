#pragma once
#include "ast.h"
#include "ast_walk.h"
#include "symbol_table.h"

typedef struct {
  SymbolTable *symtab;
  int uses_print;
  int uses_input;
} AnalysisContext;

AnalysisContext *analyse_ast(StmtList *list);
void analyse_callback(void *node, const char *kind, void *ctx);
