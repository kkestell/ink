#include "boot_info.h"
#include "uart.h"
#include "printf.h"
#include "framebuffer.h"
#include "memory.h"

void main(KernelBootInfo* boot_info)
{
    void *mem;
    uart_init();
    printf("initializing memory\n");
    initialize_memory(boot_info);
    printf("\nallocating 1 KB\n");
    mem = malloc(1024);
    print_buddy();
    printf("\nfreeing\n");
    free(mem);
    print_buddy();
    printf("\nallocating 10 KB\n");
    mem = malloc(1024 * 10);
    print_buddy();
    printf("\nfreeing\n");
    free(mem);
    print_buddy();

    while (1);
}
