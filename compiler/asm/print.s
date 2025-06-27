
section .text
global _start

_start:
    mov       rax, 1                  ; system call for write
    mov       rdi, 1                  ; file handle 1 is stdout
    mov       rsi, message            ; address of string to output
    mov       rdx, 13                 ; number of bytes / str length
    syscall                           ; invoke operating system to do the write

    mov rax, 60
    mov rdi, 1
    syscall

section .data
msg db message "Input for variable: ", 0ah
