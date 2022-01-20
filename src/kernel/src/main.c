#include <stdint.h>

#include "boot_info.h"
#include "uart.h"
#include "printf.h"
#include "fb.h"

void main(kernel_boot_info* boot_info)
{
    uart_init();

    clear(boot_info);
    julia(boot_info);

    printf("memory_map_size:          %lu\n",    boot_info->memory_map_size);
    printf("memory_map_key:           %lu\n",    boot_info->memory_map_key);
    printf("descriptor_size:          %lu\n",    boot_info->descriptor_size);
    printf("descriptor_version:       %lu\n",    boot_info->descriptor_version);
    printf("horizontal_resolution:    %lu\n",    boot_info->horizontal_resolution);
    printf("vertical_resolution:      %lu\n",    boot_info->vertical_resolution);
    printf("pixel_format:             %u\n",     boot_info->pixel_format);
    printf("pixel_bitmask:\n");
    printf("    red:                  0x%08x\n", boot_info->pixel_bitmask.red);
    printf("    green:                0x%08x\n", boot_info->pixel_bitmask.green);
    printf("    blue:                 0x%08x\n", boot_info->pixel_bitmask.blue);
    printf("    reserved:             0x%08x\n", boot_info->pixel_bitmask.reserved);
    printf("pixels_per_scan_line:     %lu\n",    boot_info->pixels_per_scan_line);
    printf("framebuffer_base_address: 0x%lx\n",  boot_info->framebuffer_base_address);
    printf("framebuffer_size:         %lu\n",    boot_info->framebuffer_size);
    printf("\n");

    while (1);
}
