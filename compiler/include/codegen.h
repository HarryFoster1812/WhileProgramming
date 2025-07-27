#pragma once
#include "ast.h"
#include "ast_analysis.h"
#include <stdio.h>

typedef struct {
  const char *name64; // 64-bit name
  const char *name32; // 32-bit name
  const char *name16; // 16-bit name
  const char *name8l; // low 8-bit name
  const char *name8h; // high 8-bit name (if exists)
  const char *role;   // Typical usage or calling convention notes
  int caller_saved;   // 1 if caller saved, 0 if callee saved
} RegisterInfo;

extern RegisterInfo registers[];

extern const int num_registers;

int gen_code(AnalysisContext *ctx, StmtList *ast, FILE *out_file);

void codegen_callback(void *node, const char *node_type, void *ctx);

void copy_file_contents(FILE *input_file, FILE *out_file);
int copy_snippet(const char *path, FILE *out_file);
