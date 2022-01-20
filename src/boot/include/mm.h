#ifndef _MM_H
#define _MM_H

#include "uefi.h"
#include "boot_info.h"

UEFI_STATUS mm_init(UINTN *memoryMapKey, KernelBootInfo *bootInfo);

#endif // _MM_H
