#pragma once

#include "efi.h"

EFI_STATUS load_kernel(EFI_PHYSICAL_ADDRESS *kernel_entry_point);
