#ifndef _LOADER_H
#define _LOADER_H

#include "efi.h"

EFI_STATUS load_kernel(EFI_PHYSICAL_ADDRESS *kernel_entry_point);

#endif // _LOADER_H
