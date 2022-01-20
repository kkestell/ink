#include "memory_map.h"
#include "printf.h"

UEFI_STATUS memory_map_init(UINTN *memoryMapKey, KernelBootInfo *bootInfo) 
{
    UEFI_STATUS status;

    status = uefiSystemTable->BootServices->GetMemoryMap(&bootInfo->memoryMapSize, NULL, memoryMapKey, &bootInfo->descriptorSize, &bootInfo->descriptorVersion);
    if (UEFI_ERROR(status))
    {
        // Always fails on the first attempt, but returns the required buffer size
        if (status != UEFI_BUFFER_TOO_SMALL)
        {
            kprintf(L"Error getting system memory map size %s\r\n", uefiErrorMessage(status));
            return status;
        }
    }

    UINTN mapSize = (bootInfo->memoryMapSize + 4095) / 4096;
    bootInfo->memoryMapSize = mapSize * 4096;

    status = uefiSystemTable->BootServices->AllocatePages(AllocateAnyPages, UefiLoaderData, mapSize, (UEFI_PHYSICAL_ADDRESS *)bootInfo->memoryMap);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Error allocating memory map buffer %s\r\n", uefiErrorMessage(status));
        return status;
    }

    status = uefiSystemTable->BootServices->GetMemoryMap(&bootInfo->memoryMapSize, bootInfo->memoryMap, memoryMapKey, &bootInfo->descriptorSize, &bootInfo->descriptorVersion);
    if (UEFI_ERROR(status))
    {
        kprintf(L"Error getting system memory map %s\r\n", uefiErrorMessage(status));
        return status;
    }

    return UEFI_SUCCESS;
}
