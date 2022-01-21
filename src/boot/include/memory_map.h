#ifndef _MEMORY_MAP_H
#define _MEMORY_MAP_H

#include "efi.h"
#include "boot_info.h"

EFI_STATUS memory_map_init(UINTN *memory_map_key, KernelBootInfo *boot_info);

#endif // _MEMORY_MAP_H
