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
    kalloc_init(&boot_info->memory_map);

    console_init(&boot_info->framebuffer);

    if (uart_init())
    {
        kprintf("UART init failed\n");
        // while (1);
    }

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
    kprintf("Starting kalloc tests...\n");

    // Test 1: Basic allocation and deallocation
    void* ptr1 = kmalloc(100);
    if (!ptr1)
    {
        kprintf("FAIL: Basic allocation failed\n");
        return;
    }
    kprintf("PASS: Basic allocation successful\n");
    kfree(ptr1);
    kprintf("PASS: Basic deallocation successful\n");

    // Test 2: Multiple allocations
    void* ptr2 = kmalloc(200);
    void* ptr3 = kmalloc(300);
    void* ptr4 = kmalloc(400);
    if (!ptr2 || !ptr3 || !ptr4)
    {
        kprintf("FAIL: Multiple allocations failed\n");
        return;
    }
    kprintf("PASS: Multiple allocations successful\n");

    // Test 3: Fragmentation and coalescing
    kfree(ptr3);  // Free the middle block
    void* ptr5 = kmalloc(250);  // Should fit in the freed space
    if (!ptr5)
    {
        kprintf("FAIL: Allocation after fragmentation failed\n");
        return;
    }
    kprintf("PASS: Allocation after fragmentation successful\n");

    // Free all blocks
    kfree(ptr2);
    kfree(ptr4);
    kfree(ptr5);

    // Test 4: Large allocation
    void* large_ptr = kmalloc(1000000);  // 1MB
    if (!large_ptr)
    {
        kprintf("FAIL: Large allocation failed\n");
        return;
    }
    kprintf("PASS: Large allocation successful\n");
    kfree(large_ptr);

    // Test 5: kcalloc
    int* int_array = (int*)kcalloc(100, sizeof(int));
    if (!int_array)
    {
        kprintf("FAIL: kcalloc failed\n");
        return;
    }
    for (int i = 0; i < 100; i++)
    {
        if (int_array[i] != 0)
        {
            kprintf("FAIL: kcalloc did not zero-initialize memory\n");
            return;
        }
    }
    kprintf("PASS: kcalloc successful and zero-initialized\n");
    kfree(int_array);

    // Test 6: krealloc
    void* realloc_ptr = kmalloc(200);
    realloc_ptr = krealloc(realloc_ptr, 400);
    if (!realloc_ptr)
    {
        kprintf("FAIL: krealloc failed\n");
        return;
    }
    kprintf("PASS: krealloc successful\n");
    kfree(realloc_ptr);

    // Final memory state
    kalloc_debug();

    kprintf("All kalloc tests completed successfully!\n");
}