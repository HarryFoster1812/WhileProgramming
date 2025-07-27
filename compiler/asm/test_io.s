section .data
i:      dq 0
i_str:  db "i: ", 0
nl:     db 10, 0

section .bss
input_buf:  resb 32
itoa_buf:   resb 21    ; enough for 64-bit number + sign + null terminator

section .text
global _start

; ----------------------------------------
; input: reads integer from stdin, stores at [rdi]
; argument: rdi = pointer to destination qword
; returns: nothing
input:
    push    rdi
    mov     eax, 0          ; sys_read
    mov     edi, 0          ; stdin fd
    mov     rsi, input_buf
    mov     edx, 32
    syscall
    pop     rdi

    mov     rcx, rsi
    add     rcx, rax        ; end pointer = start + bytes read

    xor     r9, r9          ; accumulator = 0
    mov     rdx, 1          ; sign = 1

.parse_loop:
    cmp     rsi, rcx
    je      .done

    mov     al, byte [rsi]
    cmp     al, 10          ; newline?
    je      .done
    cmp     al, 0           ; null terminator?
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

; ----------------------------------------
; itoa: convert integer in rdi to string in rsi buffer
; output: buffer pointed by rsi will contain ASCII string + null terminator
; preserves registers except rax, rdx, rcx, r8
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

; ----------------------------------------
; str_len: returns length of null-terminated string pointed to by rdi in rax
str_len:
    xor     rax, rax
.len_loop:
    cmp     byte [rdi + rax], 0
    je      .len_done
    inc     rax
    jmp     .len_loop
.len_done:
    ret

; ----------------------------------------
; print: prints null-terminated string at rdi
print:
    push    rdi
    call    str_len
    pop     rdi

    mov     rdx, rax       ; length
    mov     rax, 1         ; sys_write
    mov     rsi, rdi       ; string
    mov     rdi, 1         ; stdout
    syscall
    ret

; ----------------------------------------
; _start: test program
_start:
    lea     rdi, [rel i]
    call    input          ; read int into i

    lea     rdi, [rel i_str]
    call    print          ; print "i: "

    mov     rdi, [rel i]   ; load integer value
    lea     rsi, [rel itoa_buf]
    call    itoa           ; convert to string

    lea     rdi, [rel itoa_buf]
    call    print          ; print number string

    lea     rdi, [rel nl]
    call    print          ; print newline

    ; exit syscall
    mov     rax, 60
    xor     rdi, rdi
    syscall
