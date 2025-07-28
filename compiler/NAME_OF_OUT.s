section .data
a: dq 0
a_str: db "a: ", 0
b: dq 0
b_str: db "b: ", 0
c: dq 0
c_str: db "c: ", 0
d: dq 0
d_str: db "d: ", 0
x: dq 0
x_str: db "x: ", 0
y: dq 0
y_str: db "y: ", 0
z: dq 0
z_str: db "z: ", 0
count: dq 0
count_str: db "count: ", 0
flag: dq 0
flag_str: db "flag: ", 0
section .bss
input_buf: resb 32

section .text
global input

; void input(int* output, char* input_buf);
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
lea	rdi, [rel x]
call	input
lea	rdi, [rel y]
call	input
lea	rdi, [rel z]
call	input
lea	rdi, [rel flag]
call	input
lea	rdi, [rel count]
call	input
	mov rax, 0
	push rax
	mov rax, 0
	push rax
	mov rax, 0
	push rax
	mov rax, 3
	mov rbx, rax
	pop rax
	sub rax, rbx
	mov rbx, rax
	pop rax
	sub rax, rbx
	mov rbx, rax
	pop rax
	sub rax, rbx
	mov [x], rax
	mov rax, [x]
	push rax
	mov rax, 2
	mov rbx, rax
	pop rax
	add rax, rbx
	push rax
	mov rax, 0
	push rax
	mov rax, 4
	mov rbx, rax
	pop rax
	sub rax, rbx
	mov rbx, rax
	pop rax
	sub rax, rbx
	push rax
	mov rax, 5
	push rax
	mov rax, 0
	mov rbx, rax
	pop rax
	add rax, rbx
	push rax
	mov rax, 2
	push rax
	mov rax, 0
	mov rbx, rax
	pop rax
	sub rax, rbx
	push rax
	mov rax, 1
	mov rbx, rax
	pop rax
	add rax, rbx
	mov rbx, rax
	pop rax
	sub rax, rbx
	mov rbx, rax
	pop rax
	add rax, rbx
	mov [y], rax
	mov rax, [x]
	push rax
	mov rax, [y]
	mov rbx, rax
	pop rax
	add rax, rbx
	push rax
	mov rax, 0
	push rax
	mov rax, 1
	push rax
	mov rax, 2
	mov rbx, rax
	pop rax
	add rax, rbx
	mov rbx, rax
	pop rax
	sub rax, rbx
	push rax
	mov rax, 0
	push rax
	mov rax, 1
	mov rbx, rax
	pop rax
	sub rax, rbx
	mov rbx, rax
	pop rax
	sub rax, rbx
	mov rbx, rax
	pop rax
	add rax, rbx
	mov [z], rax
	lea	rdi, [rel x_str] 
	call	print
	mov	rdi, [rel x]
	lea	rsi, [rel itoa_buf]
	call	itoa
	lea	rdi,[rel itoa_buf]
	call	print
	lea	rdi, [rel nl]
	call	print
	lea	rdi, [rel y_str] 
	call	print
	mov	rdi, [rel y]
	lea	rsi, [rel itoa_buf]
	call	itoa
	lea	rdi,[rel itoa_buf]
	call	print
	lea	rdi, [rel nl]
	call	print
	lea	rdi, [rel z_str] 
	call	print
	mov	rdi, [rel z]
	lea	rsi, [rel itoa_buf]
	call	itoa
	lea	rdi,[rel itoa_buf]
	call	print
	lea	rdi, [rel nl]
	call	print
	mov rax, [x]
	push rax
	mov rax, [y]
	mov rbx, rax
	pop rax
	cmp rax, rbx
	mov rax, 0
	sete al
	cmp rax, 0
	mov rax, 0
	sete al
	push rax
	mov rax, [z]
	push rax
	mov rax, [x]
	push rax
	mov rax, [y]
	mov rbx, rax
	pop rax
	add rax, rbx
	mov rbx, rax
	pop rax
	cmp rax, rbx
	mov rax, 0
	setle al
	push rax
	mov rax, [x]
	push rax
	mov rax, [y]
	mov rbx, rax
	pop rax
	add rax, rbx
	push rax
	mov rax, [z]
	push rax
	mov rax, [z]
	mov rbx, rax
	pop rax
	add rax, rbx
	mov rbx, rax
	pop rax
	cmp rax, rbx
	mov rax, 0
	sete al
	cmp rax, 0
	mov rax, 0
	sete al
	pop rbx
	and al, bl
	movzx rax, al
	pop rbx
	and al, bl
	movzx rax, al
	cmp rax, 0
	je else_0
	mov rax, 1
	mov [flag], rax
	lea	rdi, [rel flag_str] 
	call	print
	mov	rdi, [rel flag]
	lea	rsi, [rel itoa_buf]
	call	itoa
	lea	rdi,[rel itoa_buf]
	call	print
	lea	rdi, [rel nl]
	call	print
	mov rax, [x]
	push rax
	mov rax, 0
	push rax
	mov rax, [y]
	mov rbx, rax
	pop rax
	sub rax, rbx
	mov rbx, rax
	pop rax
	add rax, rbx
	push rax
	mov rax, [z]
	mov rbx, rax
	pop rax
	add rax, rbx
	push rax
	mov rax, 50
	mov rbx, rax
	pop rax
	cmp rax, rbx
	mov rax, 0
	setle al
	cmp rax, 0
	je else_1
	mov rax, [x]
	push rax
	mov rax, 10
	mov rbx, rax
	pop rax
	add rax, rbx
	mov [x], rax
	mov rax, [y]
	push rax
	mov rax, 5
	mov rbx, rax
	pop rax
	sub rax, rbx
	mov [y], rax
	mov rax, [x]
	push rax
	mov rax, [y]
	mov rbx, rax
	pop rax
	add rax, rbx
	push rax
	mov rax, [z]
	mov rbx, rax
	pop rax
	add rax, rbx
	push rax
	mov rax, 0
	push rax
	mov rax, 0
	push rax
	mov rax, 3
	mov rbx, rax
	pop rax
	sub rax, rbx
	mov rbx, rax
	pop rax
	sub rax, rbx
	mov rbx, rax
	pop rax
	sub rax, rbx
	mov [z], rax
	lea	rdi, [rel x_str] 
	call	print
	mov	rdi, [rel x]
	lea	rsi, [rel itoa_buf]
	call	itoa
	lea	rdi,[rel itoa_buf]
	call	print
	lea	rdi, [rel nl]
	call	print
	lea	rdi, [rel y_str] 
	call	print
	mov	rdi, [rel y]
	lea	rsi, [rel itoa_buf]
	call	itoa
	lea	rdi,[rel itoa_buf]
	call	print
	lea	rdi, [rel nl]
	call	print
	lea	rdi, [rel z_str] 
	call	print
	mov	rdi, [rel z]
	lea	rsi, [rel itoa_buf]
	call	itoa
	lea	rdi,[rel itoa_buf]
	call	print
	lea	rdi, [rel nl]
	call	print
startwhile_2:
	mov rax, [x]
	push rax
	mov rax, [y]
	mov rbx, rax
	pop rax
	add rax, rbx
	push rax
	mov rax, [z]
	mov rbx, rax
	pop rax
	add rax, rbx
	push rax
	mov rax, 100
	mov rbx, rax
	pop rax
	cmp rax, rbx
	mov rax, 0
	setle al
	push rax
	mov rax, [flag]
	push rax
	mov rax, 0
	mov rbx, rax
	pop rax
	cmp rax, rbx
	mov rax, 0
	sete al
	cmp rax, 0
	mov rax, 0
	sete al
	pop rbx
	and al, bl
	movzx rax, al
	cmp rax, 0
	je endwhile_2
	mov rax, [x]
	push rax
	mov rax, [y]
	mov rbx, rax
	pop rax
	add rax, rbx
	mov [a], rax
	mov rax, [z]
	push rax
	mov rax, [a]
	mov rbx, rax
	pop rax
	sub rax, rbx
	mov [b], rax
	lea	rdi, [rel a_str] 
	call	print
	mov	rdi, [rel a]
	lea	rsi, [rel itoa_buf]
	call	itoa
	lea	rdi,[rel itoa_buf]
	call	print
	lea	rdi, [rel nl]
	call	print
	lea	rdi, [rel b_str] 
	call	print
	mov	rdi, [rel b]
	lea	rsi, [rel itoa_buf]
	call	itoa
	lea	rdi,[rel itoa_buf]
	call	print
	lea	rdi, [rel nl]
	call	print
	mov rax, [a]
	push rax
	mov rax, [b]
	mov rbx, rax
	pop rax
	cmp rax, rbx
	mov rax, 0
	setle al
	push rax
	mov rax, [b]
	push rax
	mov rax, 0
	mov rbx, rax
	pop rax
	cmp rax, rbx
	mov rax, 0
	sete al
	cmp rax, 0
	mov rax, 0
	sete al
	pop rbx
	and al, bl
	movzx rax, al
	cmp rax, 0
	je else_3
	jmp endif_3
else_3:
	mov rax, 0
	push rax
	mov rax, 0
	push rax
	mov rax, 1
	mov rbx, rax
	pop rax
	sub rax, rbx
	mov rbx, rax
	pop rax
	sub rax, rbx
	mov [c], rax
	mov rax, [a]
	push rax
	mov rax, [b]
	mov rbx, rax
	pop rax
	add rax, rbx
	push rax
	mov rax, [c]
	mov rbx, rax
	pop rax
	add rax, rbx
	mov [d], rax
	lea	rdi, [rel d_str] 
	call	print
	mov	rdi, [rel d]
	lea	rsi, [rel itoa_buf]
	call	itoa
	lea	rdi,[rel itoa_buf]
	call	print
	lea	rdi, [rel nl]
	call	print
startwhile_4:
	mov rax, [d]
	push rax
	mov rax, 50
	mov rbx, rax
	pop rax
	cmp rax, rbx
	mov rax, 0
	setle al
	push rax
	mov rax, [c]
	push rax
	mov rax, [d]
	mov rbx, rax
	pop rax
	add rax, rbx
	push rax
	mov rax, 100
	mov rbx, rax
	pop rax
	cmp rax, rbx
	mov rax, 0
	setle al
	pop rbx
	and al, bl
	movzx rax, al
	cmp rax, 0
	je endwhile_4
	mov rax, [d]
	push rax
	mov rax, 1
	mov rbx, rax
	pop rax
	add rax, rbx
	mov [d], rax
	lea	rdi, [rel d_str] 
	call	print
	mov	rdi, [rel d]
	lea	rsi, [rel itoa_buf]
	call	itoa
	lea	rdi,[rel itoa_buf]
	call	print
	lea	rdi, [rel nl]
	call	print
	jmp startwhile_4
endwhile_4:
endif_3:
	jmp startwhile_2
endwhile_2:
	jmp endif_1
else_1:
endif_1:
	jmp endif_0
else_0:
	mov rax, 0
	mov [flag], rax
	mov rax, [flag]
	push rax
	mov rax, 0
	mov rbx, rax
	pop rax
	cmp rax, rbx
	mov rax, 0
	sete al
	push rax
	mov rax, [x]
	push rax
	mov rax, [y]
	mov rbx, rax
	pop rax
	add rax, rbx
	push rax
	mov rax, [z]
	mov rbx, rax
	pop rax
	cmp rax, rbx
	mov rax, 0
	sete al
	cmp rax, 0
	mov rax, 0
	sete al
	pop rbx
	and al, bl
	movzx rax, al
	cmp rax, 0
	je else_5
	jmp endif_5
else_5:
	mov rax, [x]
	push rax
	mov rax, 1
	mov rbx, rax
	pop rax
	add rax, rbx
	mov [x], rax
	mov rax, [y]
	push rax
	mov rax, 1
	mov rbx, rax
	pop rax
	add rax, rbx
	mov [y], rax
endif_5:
startwhile_6:
	mov rax, [x]
	push rax
	mov rax, [y]
	mov rbx, rax
	pop rax
	cmp rax, rbx
	mov rax, 0
	setle al
	push rax
	mov rax, [y]
	push rax
	mov rax, [z]
	mov rbx, rax
	pop rax
	add rax, rbx
	push rax
	mov rax, 300
	mov rbx, rax
	pop rax
	cmp rax, rbx
	mov rax, 0
	setle al
	pop rbx
	and al, bl
	movzx rax, al
	cmp rax, 0
	je endwhile_6
	lea	rdi, [rel x_str] 
	call	print
	mov	rdi, [rel x]
	lea	rsi, [rel itoa_buf]
	call	itoa
	lea	rdi,[rel itoa_buf]
	call	print
	lea	rdi, [rel nl]
	call	print
	lea	rdi, [rel y_str] 
	call	print
	mov	rdi, [rel y]
	lea	rsi, [rel itoa_buf]
	call	itoa
	lea	rdi,[rel itoa_buf]
	call	print
	lea	rdi, [rel nl]
	call	print
	lea	rdi, [rel z_str] 
	call	print
	mov	rdi, [rel z]
	lea	rsi, [rel itoa_buf]
	call	itoa
	lea	rdi,[rel itoa_buf]
	call	print
	lea	rdi, [rel nl]
	call	print
	mov rax, [x]
	push rax
	mov rax, [y]
	mov rbx, rax
	pop rax
	add rax, rbx
	push rax
	mov rax, [z]
	mov rbx, rax
	pop rax
	add rax, rbx
	push rax
	mov rax, 250
	mov rbx, rax
	pop rax
	cmp rax, rbx
	mov rax, 0
	setle al
	cmp rax, 0
	je else_7
	mov rax, [x]
	push rax
	mov rax, 5
	mov rbx, rax
	pop rax
	add rax, rbx
	mov [x], rax
	mov rax, [y]
	push rax
	mov rax, 3
	mov rbx, rax
	pop rax
	sub rax, rbx
	mov [y], rax
	mov rax, [z]
	push rax
	mov rax, 2
	mov rbx, rax
	pop rax
	add rax, rbx
	mov [z], rax
startwhile_8:
	mov rax, [x]
	push rax
	mov rax, [y]
	mov rbx, rax
	pop rax
	add rax, rbx
	push rax
	mov rax, 100
	mov rbx, rax
	pop rax
	cmp rax, rbx
	mov rax, 0
	setle al
	cmp rax, 0
	je endwhile_8
	lea	rdi, [rel x_str] 
	call	print
	mov	rdi, [rel x]
	lea	rsi, [rel itoa_buf]
	call	itoa
	lea	rdi,[rel itoa_buf]
	call	print
	lea	rdi, [rel nl]
	call	print
	mov rax, [x]
	push rax
	mov rax, 1
	mov rbx, rax
	pop rax
	add rax, rbx
	mov [x], rax
	jmp startwhile_8
endwhile_8:
	jmp endif_7
else_7:
endif_7:
	jmp startwhile_6
endwhile_6:
endif_0:
	lea	rdi, [rel flag_str] 
	call	print
	mov	rdi, [rel flag]
	lea	rsi, [rel itoa_buf]
	call	itoa
	lea	rdi,[rel itoa_buf]
	call	print
	lea	rdi, [rel nl]
	call	print
startwhile_9:
	mov rax, [flag]
	push rax
	mov rax, 1
	mov rbx, rax
	pop rax
	cmp rax, rbx
	mov rax, 0
	sete al
	cmp rax, 0
	mov rax, 0
	sete al
	cmp rax, 0
	je endwhile_9
	mov rax, [flag]
	push rax
	mov rax, 1
	mov rbx, rax
	pop rax
	add rax, rbx
	mov [flag], rax
	mov rax, [flag]
	push rax
	mov rax, [x]
	mov rbx, rax
	pop rax
	add rax, rbx
	push rax
	mov rax, [y]
	mov rbx, rax
	pop rax
	add rax, rbx
	push rax
	mov rax, [z]
	mov rbx, rax
	pop rax
	add rax, rbx
	push rax
	mov rax, 500
	mov rbx, rax
	pop rax
	cmp rax, rbx
	mov rax, 0
	setle al
	cmp rax, 0
	je else_10
	jmp endif_10
else_10:
	lea	rdi, [rel flag_str] 
	call	print
	mov	rdi, [rel flag]
	lea	rsi, [rel itoa_buf]
	call	itoa
	lea	rdi,[rel itoa_buf]
	call	print
	lea	rdi, [rel nl]
	call	print
endif_10:
	jmp startwhile_9
endwhile_9:
	lea	rdi, [rel x_str] 
	call	print
	mov	rdi, [rel x]
	lea	rsi, [rel itoa_buf]
	call	itoa
	lea	rdi,[rel itoa_buf]
	call	print
	lea	rdi, [rel nl]
	call	print
	lea	rdi, [rel y_str] 
	call	print
	mov	rdi, [rel y]
	lea	rsi, [rel itoa_buf]
	call	itoa
	lea	rdi,[rel itoa_buf]
	call	print
	lea	rdi, [rel nl]
	call	print
	lea	rdi, [rel z_str] 
	call	print
	mov	rdi, [rel z]
	lea	rsi, [rel itoa_buf]
	call	itoa
	lea	rdi,[rel itoa_buf]
	call	print
	lea	rdi, [rel nl]
	call	print
	lea	rdi, [rel flag_str] 
	call	print
	mov	rdi, [rel flag]
	lea	rsi, [rel itoa_buf]
	call	itoa
	lea	rdi,[rel itoa_buf]
	call	print
	lea	rdi, [rel nl]
	call	print
	mov	rax, 60
	xor	rdi, rdi
	syscall
