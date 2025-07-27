section .bss
itoa_buf:   resb 21    ; enough for 64-bit number + sign + null terminator

section .text
global itoa 

itoa:
    push    rbx
    mov     rax, rdi        ; number to convert
    mov     rbx, rsi        ; buffer pointer

    cmp     rax, 0
    jge     .convert

    ; Handle negative number
    mov     byte [rbx], '-'
    inc     rbx
    neg     rax

.convert:
    lea     r8, [rbx + 20]  ; end of buffer space
    mov     byte [r8], 0    ; null terminator
    dec     r8

.convert_loop:
    xor     rdx, rdx
    mov     rcx, 10
    div     rcx             ; divide rax by 10, remainder in rdx
    add     dl, '0'
    mov     [r8], dl
    dec     r8
    test    rax, rax
    jnz     .convert_loop

    inc     r8               ; move to first digit

    ; Copy digits to start of buffer
    mov     rsi, r8         ; src
    mov     rdi, rbx        ; dest
.copy_loop:
    mov     al, [rsi]
    mov     [rdi], al
    test    al, al
    je      .done_copy
    inc     rsi
    inc     rdi
    jmp     .copy_loop

.done_copy:
    pop     rbx
    ret

