#include "mm.h"
#include "printf.h"

UEFI_STATUS mm_init(UINTN *memory_map_key, kernel_boot_info *boot_info) 
{
    UEFI_STATUS status;

    status = uefi_system_table->BootServices->GetMemoryMap(
        &boot_info->memory_map_size, 
        NULL,
        memory_map_key,
        &boot_info->descriptor_size,
        &boot_info->descriptor_version);

    if (UEFI_ERROR(status))
    {
        // Always fails on the first attempt. Returns required buffer size.
        if (status != UEFI_BUFFER_TOO_SMALL)
        {
            kprintf(L"Error getting system memory map size %s\r\n", uefi_error_message(status));
            return status;
        }
    }

    UINTN map_size = (boot_info->memory_map_size + 4095) / 4096;
    boot_info->memory_map_size = map_size * 4096;

    status = uefi_system_table->BootServices->AllocatePages(
        AllocateAnyPages,
        UefiLoaderData,
        map_size,
        (UEFI_PHYSICAL_ADDRESS *)boot_info->memory_map);

    if (UEFI_ERROR(status))
    {
        kprintf(L"Error allocating memory map buffer %s\r\n", uefi_error_message(status));
        return status;
    }

    status = uefi_system_table->BootServices->GetMemoryMap(
        &boot_info->memory_map_size,
        boot_info->memory_map,
        memory_map_key,
        &boot_info->descriptor_size,
        &boot_info->descriptor_version);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Error getting system memory map %s\r\n", uefi_error_message(status));
        return status;
    }

    return UEFI_SUCCESS;
}
