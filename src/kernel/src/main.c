#include <stdint.h>

#include "boot_info.h"
#include "uart.h"
#include "kalloc.h"
#include "kprintf.h"
#include "fb.h"

void main(boot_info_t *boot_info)
{
    uart_init();
    kalloc_init(&boot_info->memory_map);

    kprintf("MEMORY MAP\n");
    kprintf("memory_map_size:          %U\n", boot_info->memory_map.size);
    kprintf("memory_map_key:           %U\n", boot_info->memory_map.key);
    kprintf("descriptor_size:          %U\n", boot_info->memory_map.descriptor_size);
    kprintf("descriptor_version:       %u\n", boot_info->memory_map.descriptor_version);

    kprintf("FRAMEBUFFER\n");
    kprintf("horizontal_resolution:    %u\n", boot_info->framebuffer.width);
    kprintf("vertical_resolution:      %u\n", boot_info->framebuffer.height);
    kprintf("pixel_format:             %u\n", boot_info->framebuffer.pixel_format);
    kprintf("pixel_information:\n");
    kprintf("    red:                  %u\n", boot_info->framebuffer.pixel_information.red);
    kprintf("    green:                %u\n", boot_info->framebuffer.pixel_information.green);
    kprintf("    blue:                 %u\n", boot_info->framebuffer.pixel_information.blue);
    kprintf("    reserved:             %u\n", boot_info->framebuffer.pixel_information.reserved);
    kprintf("pixels_per_scan_line:     %u\n", boot_info->framebuffer.pixels_per_scan_line);
    kprintf("framebuffer_base_address: %p\n", boot_info->framebuffer.base_address);
    kprintf("framebuffer_size:         %U\n", boot_info->framebuffer.size);
    kprintf("\n");

    int *arr = (int *)kmalloc(sizeof(int) * 100);
    for (int i = 0; i < 100; i++) {
        arr[i] = i * 10;
    }
    for (int i = 0; i < 100; i++) {
        kprintf("%u ", arr[i]);
    }
    kprintf("\n");
    kfree(arr);

    clear(&boot_info->framebuffer);
    julia(&boot_info->framebuffer);

    while (1);
}
