#include "memory_map.h"
#include "printf.h"

EFI_STATUS memory_map_init(UINTN *memory_map_key, kernel_memory_info_t *memory_info) 
{
    EFI_STATUS status;

    status = efi_system_table->BootServices->GetMemoryMap(&memory_info->memory_map_size, NULL, memory_map_key, &memory_info->memory_map_descriptor_size, &memory_info->memory_map_descriptor_version);
    if (EFI_ERROR(status))
    {
        // Always fails on the first attempt, but returns the required buffer size
        if (status != EFI_BUFFER_TOO_SMALL)
        {
            kprintf(L"Error getting system memory map size %s\r\n", efi_error_message(status));
            return status;
        }
    }

    UINTN map_size = (memory_info->memory_map_size + 4095) / 4096;
    memory_info->memory_map_size = map_size * 4096;

    EFI_BS_CALL(AllocatePages(AllocateAnyPages, EfiLoaderData, map_size, (EFI_PHYSICAL_ADDRESS *)memory_info->memory_map));

    EFI_BS_CALL(GetMemoryMap(&memory_info->memory_map_size, memory_info->memory_map, memory_map_key, &memory_info->memory_map_descriptor_size, &memory_info->memory_map_descriptor_version));

    return EFI_SUCCESS;
}
