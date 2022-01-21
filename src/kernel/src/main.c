#include "boot_info.h"
#include "uart.h"
#include "printf.h"
#include "framebuffer.h"

void main(KernelBootInfo* boot_info)
{
    uart_init();
    clear(boot_info);
    while (1);
}
