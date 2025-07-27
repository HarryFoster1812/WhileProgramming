#include "codegen.h"
#include "asm_templates.h"
#include "ast.h"
#include "ast_walk.h"
#include "codegen_context.h"
#include "symbol_table.h"
#include "util.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// there is no scope/functions in while but if i need to add them later this us
// very useful
RegisterInfo registers[] = {
    {"rax", "eax", "ax", "al", "ah", "Accumulator, return value", 1},
    {"rbx", "ebx", "bx", "bl", "bh", "Callee-saved base register", 0},
    {"rcx", "ecx", "cx", "cl", "ch", "Counter, 4th arg", 1},
    {"rdx", "edx", "dx", "dl", "dh", "3rd argument", 1},
    {"rsi", "esi", "si", "sil", NULL, "2nd argument", 1},
    {"rdi", "edi", "di", "dil", NULL, "1st argument", 1},
    {"rbp", "ebp", "bp", "bpl", NULL, "Base pointer (stack frame)", 0},
    {"rsp", "esp", "sp", "spl", NULL, "Stack pointer", 0},
    {"r8", "r8d", "r8w", "r8b", NULL, "5th argument", 1},
    {"r9", "r9d", "r9w", "r9b", NULL, "6th argument", 1},
    {"r10", "r10d", "r10w", "r10b", NULL, "Caller-saved temp", 1},
    {"r11", "r11d", "r11w", "r11b", NULL, "Caller-saved temp", 1},
    {"r12", "r12d", "r12w", "r12b", NULL, "Callee-saved", 0},
    {"r13", "r13d", "r13w", "r13b", NULL, "Callee-saved", 0},
    {"r14", "r14d", "r14w", "r14b", NULL, "Callee-saved", 0},
    {"r15", "r15d", "r15w", "r15b", NULL, "Callee-saved", 0},
};

const int num_registers = sizeof(registers) / sizeof(registers[0]);

void write_variable(FILE* out_file, const char* var_name) {
    fprintf(out_file, data_var_template, var_name, "dq", "0");
    fprintf(out_file, data_var_name_template, var_name, var_name);
}

void write_input_call(FILE* out_file, const char* var_name) {
    fprintf(out_file, call_input_var, var_name);
}

void write_print_call(FILE* out_file, const char* var_name) {
    fprintf(out_file, call_print_var, var_name, var_name);
}

int gen_code(AnalysisContext* ctx, StmtList* ast, FILE* out_file) {
    // loop through all of the varibales and declare them at the top of the file
    fprintf(out_file, "section .data\n");
    SymbolTable* table = ctx->symtab;
    for (int i = 0; i < SYMBOL_TABLE_SIZE; ++i) {
        SymbolNode* bucket_node = table->buckets[i];
        if (!bucket_node)
            continue;
        while (bucket_node) {
            if (bucket_node->entry.name)
                write_variable(out_file, bucket_node->entry.name);
            bucket_node = bucket_node->next;
        }
    }

    if (ctx->uses_input) {
        if (!copy_snippet("./asm/input.s", out_file))
            return 0;
    }

    if (ctx->uses_print) {
        if (!copy_snippet("./asm/print.s", out_file) ||
            !copy_snippet("./asm/itoa.s", out_file))
            return 0;
    }

    // REAL CODE GEN

    fprintf(out_file, start_code);

    CodegenContext* context = malloc(sizeof(CodegenContext));
    if (!context) {
        panic("Failed to allocate memory for CodegenContext");
    }

    context->outfile = out_file;

    walk_stmt_list(
        ast, codegen_callback, context,
        TRAVERSAL_INORDER); // Later make ctx a state of the simulated state

    fprintf(out_file, exit_code);

    return 1;
}

void codegen_callback(void* node, const char* node_type, void* ctx) {
    if (!node) {
        return;
    }

    CodegenContext* context = (CodegenContext*)ctx;

    if (strcmp("expr", node_type) == 0) {
        Expr* expr = (Expr*)node;
        if (expr->type == EXPR_VAR) {
        }
    } else if (strcmp("stmt", node_type) == 0) {

        Stmt* stmt = (Stmt*)node;
        switch (stmt->type) {
        case STMT_INPUT:
            write_input_call(context->outfile, stmt->print_input.var_name);
            break;
        case STMT_PRINT:
            write_print_call(context->outfile, stmt->print_input.var_name);
            break;
        default:
            break;
        }
    } else if (strcmp("bool", node_type) == 0) {

        BoolExpr* bexpr = (BoolExpr*)node;
        switch (bexpr->type) {
        case BOOL_AND:
            break;
        case BOOL_EQL:
            break;
        case BOOL_FALSE:
            break;
        case BOOL_TRUE:
            break;
        case BOOL_LEQ:
            break;
        case BOOL_NOT:
            break;
        default:
            break;
        }
    }

    return;
}

void copy_file_contents(FILE* input_file, FILE* out_file) {
    char buffer[1024];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), input_file)) > 0) {
        fwrite(buffer, 1, bytes, out_file);
    }
}

int copy_snippet(const char* path, FILE* out_file) {
    FILE* input = fopen(path, "r");
    if (!input) {
        fprintf(stderr, "Error: could not open %s\n", path);
        return 0;
    }
    copy_file_contents(input, out_file);
    fclose(input);
    return 1;
}
