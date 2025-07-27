section .data
x: dq 0
x_str: db "x: ", 0
y: dq 0
y_str: db "y: ", 0
z: dq 0
z_str: db "z: ", 0
section .data
nl:     db 10, 0

section .text
global print
global strlen

strlen:
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
    call strlen       ; rax = length of string
    pop rdi              ; restore rdi (string ptr)

    mov rdx, rax         ; rdx = length
    mov rax, 1           ; syscall: write
    mov rsi, rdi         ; rsi = pointer to string
    mov rdi, 1           ; stdout (fd = 1)
    syscall
    ret
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

global _start
section .text
_start:
	mov rax, 1
push rax
	mov rax, 1
cmp rbx, 0
setne bl
cmp rax, 0
setne al
and al, bl
movzx rax, al
	push rax
	mov rax, 0
	mov rbx, rax
	pop rax
	comp rax, rbx
	mov rax, 0
	setle al
push rax
	mov rax, 1
push rax
	mov rax, 1
cmp rbx, 0
setne bl
cmp rax, 0
setne al
and al, bl
movzx rax, al

push rax
	mov rax, 0
	mov rbx, rax
	pop rax
	comp rax, rbx
	mov rax, 0
	setle al
cmp rbx, 0
setne bl
cmp rax, 0
setne al
and al, bl
movzx rax, al
	cmp rax, 0
	je else_0

	mov rax, 1
	mov [z], rax
	j endif_0

else_0:
	mov rax, 3
	mov [z], rax
endif_0:
	lea	rdi, [rel z_str] 
	call	print
	mov	rdi, [rel z]
	lea	rsi, [rel itoa_buf]
	call	itoa
	lea	rdi,[rel itoa_buf]
	call	print
	lea	rdi, [rel nl]
	call	print
	mov	rax, 60
	xor	rdi, rdi
	syscall
