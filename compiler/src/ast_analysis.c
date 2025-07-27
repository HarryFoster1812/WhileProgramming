#include "ast_analysis.h"
#include "ast.h"
#include "ast_walk.h"
#include "symbol_table.h"
#include "util.h"
#include <stdlib.h>
#include <string.h>

AnalysisContext* analyse_ast(StmtList* list) {
    AnalysisContext* ctx = calloc(1, sizeof(AnalysisContext));
    if (!ctx) {
        panic("Failed to create analysis context");
    }
    ctx->symtab = create_symbol_table();
    walk_stmt_list(list, analyse_callback, ctx, TRAVERSAL_PREORDER, 1);
    return ctx;
}

void analyse_callback(void* node, const char* kind, void* ctx) {
    if (!node) {
        return;
    }
    // node is safe to work with
    AnalysisContext* context = (AnalysisContext*)ctx;

    if (strcmp("expr", kind) == 0) {
        Expr* expr = (Expr*)node;
        if (expr->type == EXPR_VAR) {
            add_symbol(context->symtab, expr->var_name);
        }
    } else if (strcmp("stmt", kind) == 0) {

        Stmt* stmt = (Stmt*)node;
        switch (stmt->type) {
        case STMT_INPUT:
            context->uses_input = 1;
            if (stmt->print_input.var_name) {
                add_symbol(context->symtab, stmt->print_input.var_name);
            }
            break;
        case STMT_PRINT:
            context->uses_print = 1;
            if (stmt->print_input.var_name) {
                add_symbol(context->symtab, stmt->print_input.var_name);
            }
            break;
        default:
            break;
        }
    }

    return;
}
