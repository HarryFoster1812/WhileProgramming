.data
i: dd 0
section .text
global input

input:
    push rdi
    mov     eax, 0              ; syscall number for sys_read
    mov     edi, 0              ; file descriptor: stdin
    mov     rsi, input_buf      ; buffer to read input
    mov     edx, 32             ; number of bytes
    syscall
    pop     rdi 

    ; rsi = pointer to input_buf
    ; rdi = pointer to where the result should be stored

    mov     rcx, rsi            ; rcx = current position in buffer
    add     rcx, rax            ; rcx = end of input

    xor     r9, r9              ; accumulator = 0
    mov     rdx, 1              ; sign = 1

.parse_loop:
    cmp     rsi, rcx
    je      .done

    mov     al, byte [rsi]
    cmp     al, 10              ; newline
    je      .done
    cmp     al, 0               ; null terminator
    je      .done

    cmp     al, '-'
    jne     .not_negative
    mov     rdx, -1
    inc     rsi
    jmp     .parse_loop

.not_negative:
    imul    r9, 10
    sub     al, '0'
    movzx   r8, al
    add     r9, r8

    inc     rsi
    jmp     .parse_loop

.done:
    imul    r9, rdx
    mov     [rdi], r9
    ret

section .bss
input_buf: resb 32
section .text
global print
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
global _start
section .text
_start:
mov rax, 60
xor rdi, rdi
syscall
