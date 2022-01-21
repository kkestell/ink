#ifndef _BOOT_INFO_H
#define _BOOT_INFO_H

#include <stdint.h>

typedef struct PixelBitmask {
    uint32_t red;
    uint32_t green;
    uint32_t blue;
    uint32_t reserved;
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
    uint32_t  type;
    uintptr_t physical_start;
    uintptr_t virtual_start;
    uint64_t  page_count;
    uint64_t  attributes;
} MemoryMapDescriptor;

typedef struct KernelBootInfo
{
    MemoryMapDescriptor* memory_map;
    uint64_t             memory_map_size;
    uint64_t             memory_map_key;
    uint64_t             memory_map_descriptor_size;
    uint32_t             memory_map_descriptor_version;
    uint32_t             framebuffer_width;
    uint32_t             framebuffer_height;
    PixelFormat          framebuffer_pixel_format;
    PixelBitmask         framebuffer_pixel_bitmask;
    uint32_t             framebuffer_pixels_per_scan_line;
    uint64_t             framebuffer_base_address;
    uint64_t             framebuffer_size;
} KernelBootInfo;

#endif // _BOOT_INFO_H
