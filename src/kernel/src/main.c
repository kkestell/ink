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

    printf("memoryMapSize:          %lu\n",    boot_info->memoryMapSize);
    printf("memoryMapKey:           %lu\n",    boot_info->memoryMapKey);
    printf("descriptorSize:          %lu\n",    boot_info->descriptorSize);
    printf("descriptorVersion:       %lu\n",    boot_info->descriptorVersion);
    printf("horizontalResolution:    %lu\n",    boot_info->horizontalResolution);
    printf("verticalResolution:      %lu\n",    boot_info->verticalResolution);
    printf("pixelFormat:             %u\n",     boot_info->pixelFormat);
    printf("pixel_bitmask:\n");
    printf("    red:                  0x%08x\n", boot_info->pixel_bitmask.red);
    printf("    green:                0x%08x\n", boot_info->pixel_bitmask.green);
    printf("    blue:                 0x%08x\n", boot_info->pixel_bitmask.blue);
    printf("    reserved:             0x%08x\n", boot_info->pixel_bitmask.reserved);
    printf("pixelsPerScanLine:     %lu\n",    boot_info->pixelsPerScanLine);
    printf("framebufferBaseAddress: 0x%lx\n",  boot_info->framebufferBaseAddress);
    printf("framebufferSize:         %lu\n",    boot_info->framebufferSize);
    printf("\n");

    while (1);
}
