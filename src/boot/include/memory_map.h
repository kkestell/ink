#ifndef _MEMORY_MAP_H
#define _MEMORY_MAP_H

#include "uefi.h"
#include "boot_info.h"

UEFI_STATUS memory_map_init(UINTN *memoryMapKey, KernelBootInfo *bootInfo);

#endif // _MEMORY_MAP_H
