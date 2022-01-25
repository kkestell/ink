#ifndef _FRAMEBUFFER_H
#define _FRAMEBUFFER_H

#include "efi.h"
#include "boot_info.h"

EFI_STATUS framebuffer_init(kernel_framebuffer_info_t *framebuffer_info);

#endif // _FRAMEBUFFER_H
