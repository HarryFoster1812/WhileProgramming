section .text
global allocate

; void print(char *rdi)
allocate:
    ; mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)
    mov rax, 9          ; syscall: mmap
    xor rdi, rdi        ; NULL addr = let kernel choose
    mov rsi, 4096       ; size = 4 KB
    mov rdx, 3          ; PROT_READ | PROT_WRITE = 1 | 2 = 3
    mov r10, 34         ; MAP_PRIVATE | MAP_ANONYMOUS = 0x02 | 0x20 = 34
    mov r8, -1          ; fd = -1
    xor r9, r9          ; offset = 0
    syscall
    ret
    ; result (allocated memory pointer) is in RAX
