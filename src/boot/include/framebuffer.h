#ifndef _FRAMEBUFFER_H
#define _FRAMEBUFFER_H

#include "uefi.h"
#include "boot_info.h"

UEFI_STATUS framebuffer_init(KernelBootInfo *bootInfo);

#endif // _FRAMEBUFFER_H
