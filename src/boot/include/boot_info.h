#pragma once

#include "efi.h"

typedef struct kernel_framebuffer_info
{
    uint32_t                  framebuffer_width;
    uint32_t                  framebuffer_height;
    EFI_GRAPHICS_PIXEL_FORMAT framebuffer_pixel_format;
    EFI_PIXEL_BITMASK         framebuffer_pixel_information;
    uint32_t                  framebuffer_pixels_per_scan_line;
    uint64_t                  framebuffer_base_address;
    uint64_t                  framebuffer_size;
} kernel_framebuffer_info_t;

typedef struct kernel_memory_info
{
    EFI_MEMORY_DESCRIPTOR     *memory_map;
    uint64_t                  memory_map_size;
    uint64_t                  memory_map_key;
    uint64_t                  memory_map_descriptor_size;
    uint32_t                  memory_map_descriptor_version;
} kernel_memory_info_t;

typedef struct kernel_boot_info
{
    kernel_memory_info_t memory;
    kernel_framebuffer_info_t framebuffer;
} kernel_boot_info_t;
