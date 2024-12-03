.section .text
.global _start

_start:
/* syscall write(int fd, const void *buf, size_t count) */
    movq $1, %rdi
    movq $msg, %rsi
    movq $len, %rdx
    movq $1, %rax
    syscall

/* syscall exit(int status) */
    xorq %rdi, %rdi
    movq $231, %rax
    syscall

msg:
.ascii "Hello, x86-64!\n"
len = . - msg
