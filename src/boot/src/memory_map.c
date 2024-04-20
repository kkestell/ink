#include "memory_map.h"
#include "kprintf.h"

EFI_STATUS memory_map_init(UINTN *memory_map_key, kernel_memory_info_t *memory_info)
{
    EFI_STATUS status;

    memory_info->memory_map_size = 0;

    status = efi_system_table->BootServices->GetMemoryMap(&memory_info->memory_map_size, NULL, memory_map_key, &memory_info->memory_map_descriptor_size, &memory_info->memory_map_descriptor_version);
    if (status != EFI_BUFFER_TOO_SMALL)
    {
        kprintf(L"Error getting system memory map size: %s\r\n", efi_error_message(status));
        return status;
    }

    memory_info->memory_map_size = ((memory_info->memory_map_size + 4095) / 4096) * 4096;
    EFI_BS_CALL(AllocatePages(AllocateAnyPages, EfiLoaderData, EFI_SIZE_TO_PAGES(memory_info->memory_map_size), (EFI_PHYSICAL_ADDRESS *)&memory_info->memory_map));
    EFI_BS_CALL(GetMemoryMap(&memory_info->memory_map_size, memory_info->memory_map, memory_map_key, &memory_info->memory_map_descriptor_size, &memory_info->memory_map_descriptor_version));

    return EFI_SUCCESS;
}

