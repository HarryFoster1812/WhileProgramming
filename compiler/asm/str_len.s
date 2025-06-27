
; Function
my_strlen:
    pop rdi             ; get argument from stack
    xor rax, rax
.loop:
    cmp byte [rdi + rax], 0
    je .done
    inc rax
    jmp .loop
.done:
    ret

SAMPLE USEAGE:   
    push msg        ; push address of string
    call my_strlen  ; call function
    add rsp, 8      ; clean up the stack (remove pushed arg)

    ; write(1, msg, rax)
    mov rdx, rax       ; length of string
    mov rax, 1         ; syscall number for write
    mov rdi, 1         ; file descriptor (stdout)
    mov rsi, msg       ; message to print
    syscall

