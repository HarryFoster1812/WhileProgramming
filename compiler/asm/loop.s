section .text
global _start

_start:
    mov rax, 60
    mov rdi, 1
    syscall

    loop_1:
        # do condition
        comp #reg, 1
        je exit_loop_1
        # do inner loop
        j loop_1
    exit_loop_1:
    # after loop
