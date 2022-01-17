#include "uefi.h"
#include "utils.h"

void* image_handle;

void puts(void *p, uint16_t c)
{
    uint16_t chars[2] = { c, '\0' };
    uefi_system_table->ConOut->OutputString(uefi_system_table->ConOut, (uint16_t*)chars);
}

void uefi_init(void *ih, UEFI_SYSTEM_TABLE *st)
{
    image_handle = ih;
    uefi_system_table = st;

    kprintf_init(0, puts);
}
