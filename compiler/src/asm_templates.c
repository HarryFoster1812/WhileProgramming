#include "asm_templates.h"

const char* start_code = "global _start\n"
                         "section .text\n"
                         "_start:\n";

const char* exit_code = "mov rax, 60\n"
                        "xor rdi, rdi\n"
                        "syscall\n";

const char* data_var_template =
    "%s: %s %s\n"; // e.g. label: db "Hello, world!", 0
