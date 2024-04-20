#include "boot_info.h"
#include "framebuffer.h"
#include "loader.h"
#include "memory_map.h"
#include "kprintf.h"

EFI_STATUS efi_main(void *image_handle, EFI_SYSTEM_TABLE *system_table)
{
    EFI_STATUS status;
    kernel_boot_info_t boot_info;

    efi_init(image_handle, system_table);

    kprintf(L"A\r\n");

    // Initialize framebuffer

    status = framebuffer_init(&boot_info.framebuffer);
    if (EFI_ERROR(status))
    {
        die(L"Error initializing framebuffer %s\r\n", efi_error_message(status));
        return status;
    }

    kprintf(L"B\r\n");

    // Load kernel

    EFI_PHYSICAL_ADDRESS *kernel_entry_point = 0;
    status = load_kernel(kernel_entry_point);
    if (EFI_ERROR(status))
    {
        die(L"Error loading kernel %s\r\n", efi_error_message(status));
        return status;
    }

    kprintf(L"C\r\n");

    // Map memory

    UINTN memory_map_key;
    status = memory_map_init(&memory_map_key, &boot_info.memory);
    if (EFI_ERROR(status))
    {
        die(L"Error initializing memory map %s\r\n", efi_error_message(status));
        return status;
    }

    kprintf(L"D\r\n");

    // Exit boot services

    EFI_BS_CALL(ExitBootServices(image_handle, memory_map_key));

    // Jump to kernel

    void (*kernel_entry)(kernel_boot_info_t *) = (void(*)(kernel_boot_info_t *))*kernel_entry_point;
    kernel_entry(&boot_info);

    return EFI_LOAD_ERROR;
}
