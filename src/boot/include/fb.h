#ifndef _FB_H
#define _FB_H

#include "uefi.h"
#include "boot_info.h"

UEFI_STATUS fb_init(kernel_boot_info *boot_info);

#endif // _FB_H
