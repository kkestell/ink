#pragma once

#include "types.h"

typedef struct PixelBitmask {
    u32 red;
    u32 green;
    u32 blue;
    u32 reserved;
} PixelBitmask;

typedef enum PixelFormat {
    PixelRedGreenBlueReserved8BitPerColor,
    PixelBlueGreenRedReserved8BitPerColor,
    PixelBitMask,
    PixelBltOnly,
    PixelFormatMax
} PixelFormat;

typedef struct MemoryMapDescriptor
{
    u32  type;
    void *physical_start;
    void *virtual_start;
    u64  page_count;
    u64  attributes;
} MemoryMapDescriptor;

typedef struct KernelBootInfo
{
    MemoryMapDescriptor* memory_map;
    u64             memory_map_size;
    u64             memory_map_key;
    u64             memory_map_descriptor_size;
    u32             memory_map_descriptor_version;
    u32             framebuffer_width;
    u32             framebuffer_height;
    PixelFormat          framebuffer_pixel_format;
    PixelBitmask         framebuffer_pixel_bitmask;
    u32             framebuffer_pixels_per_scan_line;
    u64             framebuffer_base_address;
    u64             framebuffer_size;
} KernelBootInfo;
