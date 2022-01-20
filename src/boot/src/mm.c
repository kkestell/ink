#include "mm.h"
#include "printf.h"

UEFI_STATUS mm_init(UINTN* memoryMapKey, KernelBootInfo* bootInfo) 
{
    UEFI_STATUS status;

    status = uefi_system_table->BootServices->GetMemoryMap(
        &bootInfo->memoryMapSize, 
        NULL,
        memoryMapKey,
        &bootInfo->descriptorSize,
        &bootInfo->descriptorVersion);

    if (UEFI_ERROR(status))
    {
        // Always fails on the first attempt, but returns the required buffer size
        if (status != UEFI_BUFFER_TOO_SMALL)
        {
            kprintf(L"Error getting system memory map size %s\r\n", uefi_error_message(status));
            return status;
        }
    }

    UINTN map_size = (bootInfo->memoryMapSize + 4095) / 4096;
    bootInfo->memoryMapSize = map_size * 4096;

    status = uefi_system_table->BootServices->AllocatePages(
        AllocateAnyPages,
        UefiLoaderData,
        map_size,
        (UEFI_PHYSICAL_ADDRESS*)bootInfo->memory_map);

    if (UEFI_ERROR(status))
    {
        kprintf(L"Error allocating memory map buffer %s\r\n", uefi_error_message(status));
        return status;
    }

    status = uefi_system_table->BootServices->GetMemoryMap(
        &bootInfo->memoryMapSize,
        bootInfo->memory_map,
        memoryMapKey,
        &bootInfo->descriptorSize,
        &bootInfo->descriptorVersion);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Error getting system memory map %s\r\n", uefi_error_message(status));
        return status;
    }

    return UEFI_SUCCESS;
}
