#pragma once

#include "efi.h"
#include "boot_info.h"

EFI_STATUS memory_map_init(UINTN *memory_map_key, kernel_memory_info_t *memory_info);
