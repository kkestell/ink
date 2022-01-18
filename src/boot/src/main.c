#include "boot_info.h"
#include "fb.h"
#include "loader.h"
#include "mm.h"
#include "printf.h"

UEFI_STATUS uefi_main(void *image_handle, UEFI_SYSTEM_TABLE *system_table)
{
    UEFI_STATUS status;
    kernel_boot_info boot_info;

    uefi_init(image_handle, system_table);

    // framebuffer

    status = fb_init(&boot_info);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Error initializing framebuffer %s\r\n", uefi_error_message(status));
        return status;
    }

    // load kernel

    UEFI_PHYSICAL_ADDRESS *kernel_entry_point = 0;
    status = load_kernel(kernel_entry_point);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Error loading kernel %s\r\n", uefi_error_message(status));
        return status;
    }

    // map memory

    UINTN memory_map_key;
    status = mm_init(&memory_map_key, &boot_info);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Error initializing memory map %s\r\n", uefi_error_message(status));
        return status;
    }

    // exit boot services

    status = uefi_system_table->BootServices->ExitBootServices(image_handle, memory_map_key);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Error exiting boot services %s\r\n", uefi_error_message(status));
        return status;
    }

    // jump to kernel

    void (*kernel_entry)(kernel_boot_info *);

    kernel_entry = (void (*)(kernel_boot_info *))*kernel_entry_point;
    kernel_entry(&boot_info);

    return UEFI_LOAD_ERROR;
}
