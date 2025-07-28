#include "asm_templates.h"

const char* start_code = "global _start\n"
                         "section .text\n"
                         "_start:\n";

const char* exit_code = "\tmov\trax, 60\n"
                        "\txor\trdi, rdi\n"
                        "\tsyscall\n";

const char* data_var_template =
    "%s: %s %s\n"; // e.g. label: db "Hello, world!", 0

const char* data_var_name_template =
    "%s_str: db \"%s: \", 0\n"; // e.g. label: db "Hello, world!", 0

const char* call_print_var = "\tlea\trdi, [rel %s_str] \n"
                             "\tcall\tprint\n"
                             "\tmov\trdi, [rel %s]\n"
                             "\tlea\trsi, [rel itoa_buf]\n"
                             "\tcall\titoa\n"
                             "\tlea\trdi,[rel itoa_buf]\n"
                             "\tcall\tprint\n"
                             "\tlea\trdi, [rel nl]\n"
                             "\tcall\tprint\n";

const char* call_input_var = "lea\trdi, [rel %s]\n"
                             "call\tinput\n";
