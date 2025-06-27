
section .text
global _start



_start:

    push varPtr


    mov rax, 60
    mov rdi, 1
    syscall

section  .data
    varPtr: dq 0
