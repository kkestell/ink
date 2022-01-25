.text

.globl _start

_start:
    mov %rcx, %rdi
    callq main

_exit:
