section .text
global print

; void print(char *rdi)
print:
    push rdi             ; save rdi (argument)
    call my_strlen       ; rax = length of string
    pop rdi              ; restore rdi (string ptr)

    mov rdx, rax         ; rdx = length
    mov rax, 1           ; syscall: write
    mov rsi, rdi         ; rsi = pointer to string
    mov rdi, 1           ; stdout (fd = 1)
    syscall
    ret
