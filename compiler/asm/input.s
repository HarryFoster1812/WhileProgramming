section .text
global _start

input:
    push rdi
    mov     eax, 0              ; syscall number for sys_read
    mov     edi, 0              ; file descriptor: stdin
    mov     rsi, input_buf      ; buffer to read input
    mov     edx, 32             ; number of bytes
    syscall

    pop rdi 

    ; rsi = pointer to input_buf
    ; rdi = pointer to where the result should be stored (passed via rdi)


    mov     rcx, rsi            ; rcx = current position in buffer
    add     rcx, rax            ; rcx = end of input

    xor     r9, r9              ; accumulator for result = 0
    mov     rdx, 1              ; sign = 1 by default

.parse_loop:
    cmp     rsi, rcx            ; reached end?
    je      .done

    mov     al, byte [rsi]      ; get current character
    cmp     al, 10              ; newline?
    je      .done
    cmp     al, 0               ; null terminator?
    je      .done

    cmp     al, '-'             ; negative sign?
    jne     .not_negative
    mov     rdx, -1
    inc     rsi
    jmp     .parse_loop

.not_negative:
    ; cmp     al, '0'
    ; jl      .done
    ; cmp     al, '9'
    ; jg      .done

    imul    r9, 10
    sub     al, '0'
    movzx   r8, al
    add     r9, r8

    inc     rsi
    jmp     .parse_loop

.done:
    imul    r9, rdx         ; apply sign
    mov     [rdi], r9           ; store result in [rdi]
    ret

_start:
    lea rdi, varPtr         ; pass pointer to store result
    call    input

    ; For demo: exit code = parsed value (lower 32 bits only)
    mov     rax, 60             ; syscall: exit
    mov     rdi, [varPtr] ; exit code = parsed number
    syscall

section .bss
input_buf: resb 32              ; reserve 32 bytes for input

section .data
varPtr: dq 0                    ; store the parsed long value
