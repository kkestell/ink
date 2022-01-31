.text

.globl _start

_start:
    movq %rcx, %r15
    callq start
    hlt
