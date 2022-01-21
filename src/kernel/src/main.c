#include "boot_info.h"
#include "uart.h"
#include "printf.h"
#include "framebuffer.h"

static char *_mem_attribute[] = {
    "reserved",
    "loader code",
    "loader data",
    "boot services code",
    "boot services data",
    "runtime services code",
    "runtime services data",
    "conventional memory",
    "unusable memory",
    "acpi reclaim memory",
    "acpi memory nvs",
    "memory mapped io",
    "memory mapped io port space",
    "pal code",
    "persistent memory"
};

#define EfiMaxMemoryType 0x0000000e

static void mm_debug(KernelBootInfo* boot_info) 
{
    MemoryMapDescriptor *d = boot_info->memory_map;

    uint64_t total_mapped = 0;

    printf("PHYSICAL ADDRESS     PAGES    TYPE\n");
    for (uint64_t i = 0; i < boot_info->memory_map_size / boot_info->memory_map_descriptor_size; i++) 
    {
        printf("%16lx", d->physical_start);
        printf("    ");
        printf("%6lu", d->page_count);
        printf("    ");

        if (d->type >= EfiMaxMemoryType)
        {
            printf("unrecognized");
        } 
        else
        {
            printf(_mem_attribute[d->type]);
        }

        total_mapped += d->count * 4096;
        uint8_t *ptr = ((uint8_t *)d);
        ptr += boot_info->memory_map_descriptor_size;
        d = (MemoryMapDescriptor *)ptr;

        printf("\n");
    }

    printf("\n");
    printf("%lu bytes mapped", total_mapped);
    printf("\n");
}

void main(KernelBootInfo* boot_info)
{
    uart_init();
    mm_debug(boot_info);
    clear(boot_info);
    while (1);
}
