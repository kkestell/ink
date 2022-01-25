.text

.globl _start

_start:
    movq %rcx, %r15
    callq NimMain
    hlt
