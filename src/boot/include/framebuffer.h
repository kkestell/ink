#pragma once

#include "efi.h"
#include "boot_info.h"

EFI_STATUS framebuffer_init(kernel_framebuffer_info_t *framebuffer_info);
