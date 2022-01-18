#ifndef _BOOT_INFO_H
#define _BOOT_INFO_H

#include "uefi.h"

typedef struct kernel_boot_info
{
    // mm
    UEFI_MEMORY_DESCRIPTOR *memory_map;
    uint64_t memory_map_size;
    uint64_t memory_map_key;
    uint64_t descriptor_size;
    uint32_t descriptor_version;

    // fb
    uint32_t horizontal_resolution;
    uint32_t vertical_resolution;
    UEFI_GRAPHICS_PIXEL_FORMAT pixel_format;
    UEFI_PIXEL_BITMASK pixel_information;
    uint32_t pixels_per_scan_line;
    uint64_t framebuffer_base_address;
    uint64_t framebuffer_size;
} kernel_boot_info;

#endif // _BOOT_INFO_H
