#include <stdint.h>

#include "boot_info.h"
#include "uart.h"
#include "kalloc.h"
#include "kprintf.h"
#include "fb.h"
#include "console.h"

void debug_bootinfo(boot_info_t *boot_info);
void test_kalloc();

void main(boot_info_t *boot_info)
{   
    console_init(&boot_info->framebuffer);

    if (uart_init())
    {
        kprintf("UART init failed\n");
        // while (1);
    }

    kalloc_init(&boot_info->memory_map);

    debug_bootinfo(boot_info);

    test_kalloc();

    while (1);
}

void debug_bootinfo(boot_info_t *boot_info)
{
    kprintf("--------------------------------------------------------------------------------\n");
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
    kprintf("--------------------------------------------------------------------------------\n");
}

void test_kalloc()
{
    // Initially allocate memory for 100 integers using kcalloc to ensure zero-initialization
    int *arr = (int *)kcalloc(100, sizeof(int));
    kprintf("Initial array (zero-initialized):\n");
    for (int i = 0; i < 100; i++)
    {
        kprintf("%u ", arr[i]); // Should print 0s
    }
    kprintf("\n");

    // Populate the array with values
    for (int i = 0; i < 100; i++)
    {
        arr[i] = i;
    }

    // Print the populated array
    kprintf("Populated array:\n");
    for (int i = 0; i < 100; i++)
    {
        kprintf("%u ", arr[i]); // Should print 0 to 99
    }
    kprintf("\n");

    // Resize the array to 150 integers using krealloc
    arr = (int *)krealloc(arr, sizeof(int) * 150);
    kprintf("Resized array (new elements are uninitialized):\n");
    for (int i = 100; i < 150; i++)
    {
        arr[i] = i * 2; // Initialize new elements with some values
    }
    for (int i = 0; i < 150; i++)
    {
        kprintf("%u ", arr[i]); // Should print 0 to 99, followed by 200 to 298
    }
    kprintf("\n");

    // Further reduce the size of the array to 50 integers using krealloc
    arr = (int *)krealloc(arr, sizeof(int) * 50);
    kprintf("Reduced array size:\n");
    for (int i = 0; i < 50; i++)
    {
        kprintf("%u ", arr[i]); // Should print 0 to 49
    }
    kprintf("\n");

    kfree(arr);
}
