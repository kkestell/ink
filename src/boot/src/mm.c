#include "uefi.h"
#include "utils.h"

#define NULL (void *)0

UINTN   memory_map_key;

static UEFI_MEMORY_DESCRIPTOR   *_mem_map = NULL;
static UINTN                    _mem_map_size = 0;
static UINTN                    _mem_map_desc_size = 0;
static uint64_t                 _mem_map_num_entries = 0;
static uint64_t                 _mem_map_alloced_pages = 0;
static UINT32                   _mem_map_desc_ver = 0;

static CHAR16 *_mem_attribute[] = {
    L"reserved",
    L"loader code",
    L"loader data",
    L"boot services code",
    L"boot services data",
    L"runtime services code",
    L"runtime services data",
    L"conventional memory",
    L"unusable memory",
    L"acpi reclaim memory",
    L"acpi memory nvs",
    L"memory mapped io",
    L"memory mapped io port space",
    L"pal code",
    L"persistent memory"
};

static void mm_debug(void) 
{
    void                    *mm = _mem_map;
    UEFI_MEMORY_DESCRIPTOR  *mem_map;
    uint64_t                i;
    uint64_t                total_mapped = 0;

    _mem_map_num_entries = _mem_map_size / _mem_map_desc_size;
    kprintf(L"PHYSICAL ADDRESS     VIRTUAL ADDRESS     PAGES    TYPE\r\n");
    for (i = 0; i < _mem_map_num_entries; i++) {
        mem_map = (UEFI_MEMORY_DESCRIPTOR *)mm;
        kprintf(L"%16x", mem_map->PhysicalStart);
        kprintf(L"    ");
        kprintf(L"%16x", mem_map->VirtualStart);
        kprintf(L"    ");
        kprintf(L"%6u", mem_map->NumberOfPages);
        kprintf(L"    ");
        if (mem_map->Type >= UefiMaxMemoryType) {
            kprintf(L"unrecognized");
        } else {
            kprintf(_mem_attribute[mem_map->Type]);
        }
        total_mapped += mem_map->NumberOfPages * 4096;
        mm += _mem_map_desc_size;
        kprintf(L"\r\n");
    }
    kprintf(L"\r\n");
    kprintf(L"%u bytes mapped", total_mapped);
    kprintf(L"\r\n");
}

int mm_init(void) 
{
    UINTN map_size;

    if (uefi_system_table->BootServices->GetMemoryMap(&_mem_map_size, NULL, &memory_map_key, &_mem_map_desc_size, &_mem_map_desc_ver) != UEFI_BUFFER_TOO_SMALL)
    {
        kprintf(L"Error getting system memory map");
        return 1;
    }

    map_size = (_mem_map_size + 4095) / 4096;
    _mem_map_alloced_pages = map_size;
    _mem_map_size = map_size * 4096;

    if (uefi_system_table->BootServices->AllocatePages(AllocateAnyPages, UefiLoaderData, map_size, (UEFI_PHYSICAL_ADDRESS *)&_mem_map) != UEFI_SUCCESS)
    {
        kprintf(L"Error allocating %d pages for GetMemoryMap");
        return 1;
    }

    if (uefi_system_table->BootServices->GetMemoryMap(&_mem_map_size, _mem_map, &memory_map_key, &_mem_map_desc_size, &_mem_map_desc_ver) != UEFI_SUCCESS)
    {
        kprintf(L"Failed to get system memory map");
        return 1;
    }

    mm_debug();

    return 0;
}
