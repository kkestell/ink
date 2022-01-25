#include "boot.h"

uintptr_t get_boot_info()
{
    uintptr_t i;
    asm("\t movq %%r15,%0" : "=r"(i));
    return i;
}