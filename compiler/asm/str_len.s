section .text
global my_strlen

my_strlen:
    xor rax, rax        ; length counter

.loop:
    cmp byte [rdi + rax], 0
    je .done
    inc rax
    jmp .loop

.done:
    ret
