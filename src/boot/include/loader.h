#ifndef _LOADER_H
#define _LOADER_H

#include "uefi.h"

UEFI_STATUS load_kernel(UEFI_PHYSICAL_ADDRESS *kernelEntryPoint);

#endif // _LOADER_H
