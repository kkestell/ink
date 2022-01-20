#include "boot_info.h"
#include "uart.h"
#include "printf.h"
#include "fb.h"

void main(KernelBootInfo* bootInfo)
{
    uart_init();
    clear(bootInfo);
    while (1);
}
