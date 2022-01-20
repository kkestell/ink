#include "boot_info.h"
#include "fb.h"
#include "loader.h"
#include "mm.h"
#include "printf.h"

UEFI_STATUS uefi_main(void* imageHandle, UEFI_SYSTEM_TABLE* systemTable)
{
    UEFI_STATUS status;
    KernelBootInfo bootInfo;

    uefi_init(imageHandle, systemTable);

    // framebuffer

    status = fb_init(&bootInfo);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Error initializing framebuffer %s\r\n", uefi_error_message(status));
        return status;
    }

    // load kernel

    UEFI_PHYSICAL_ADDRESS *kernelEntryPoint = 0;
    status = load_kernel(kernelEntryPoint);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Error loading kernel %s\r\n", uefi_error_message(status));
        return status;
    }

    // map memory

    UINTN memoryMapKey;
    status = mm_init(&memoryMapKey, &bootInfo);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Error initializing memory map %s\r\n", uefi_error_message(status));
        return status;
    }

    // exit boot services

    status = uefi_system_table->BootServices->ExitBootServices(imageHandle, memoryMapKey);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Error exiting boot services %s\r\n", uefi_error_message(status));
        return status;
    }

    // jump to kernel

    void (*kernelEntry)(KernelBootInfo *) = (void(*)(KernelBootInfo*))*kernelEntryPoint;
    kernelEntry(&bootInfo);

    return UEFI_LOAD_ERROR;
}
