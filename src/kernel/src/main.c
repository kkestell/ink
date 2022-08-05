#include <stdint.h>

#include "boot_info.h"
#include "uart.h"
#include "kalloc.h"
#include "kprintf.h"
#include "fb.h"

void main(kernel_boot_info *boot_info)
{
    uart_init();
    kalloc_init(boot_info);

    kprintf("MEMORY MAP\n");
    kprintf("memory_map_size:          %U\n", boot_info->memory_map_size);
    kprintf("memory_map_key:           %U\n", boot_info->memory_map_key);
    kprintf("descriptor_size:          %U\n", boot_info->memory_map_descriptor_size);
    kprintf("descriptor_version:       %u\n", boot_info->memory_map_descriptor_version);

    kprintf("FRAMEBUFFER\n");
    kprintf("horizontal_resolution:    %u\n", boot_info->horizontal_resolution);
    kprintf("vertical_resolution:      %u\n", boot_info->vertical_resolution);
    kprintf("pixel_format:             %u\n", boot_info->pixel_format);
    kprintf("pixel_bitmask:\n");
    kprintf("    red:                  %u\n", boot_info->pixel_bitmask.red);
    kprintf("    green:                %u\n", boot_info->pixel_bitmask.green);
    kprintf("    blue:                 %u\n", boot_info->pixel_bitmask.blue);
    kprintf("    reserved:             %u\n", boot_info->pixel_bitmask.reserved);
    kprintf("pixels_per_scan_line:     %u\n", boot_info->pixels_per_scan_line);
    kprintf("framebuffer_base_address: %p\n", boot_info->framebuffer_base_address);
    kprintf("framebuffer_size:         %U\n", boot_info->framebuffer_size);
    kprintf("\n");

    clear(boot_info);
    julia(boot_info);

    while (1);
}
