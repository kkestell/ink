#ifndef _BOOT_INFO_H
#define _BOOT_INFO_H

#include <stdint.h>

typedef struct pixel_bitmask {
    uint32_t  red;
    uint32_t  green;
    uint32_t  blue;
    uint32_t  reserved;
} pixel_bitmask;

typedef enum pixel_format {
    PixelRedGreenBlueReserved8BitPerColor,
    PixelBlueGreenRedReserved8BitPerColor,
    PixelBitMask,
    PixelBltOnly,
    PixelFormatMax
} pixel_format;

typedef struct memory_map_descriptor
{
    uint32_t  type;
    uintptr_t physical_start;
    uintptr_t virtual_start;
    uint64_t  page_count;
    uint64_t  attributes;
} memory_map_descriptor;

typedef struct kernel_boot_info
{
    // mm
    uint64_t *memory_map;
    uint64_t memory_map_size;
    uint64_t memory_map_key;
    uint64_t memory_map_descriptor_size;
    uint32_t memory_map_descriptor_version;

    // fb
    uint32_t horizontal_resolution;
    uint32_t vertical_resolution;
    pixel_format pixel_format;
    pixel_bitmask pixel_bitmask;
    uint32_t pixels_per_scan_line;
    uint64_t framebuffer_base_address;
    uint64_t framebuffer_size;
} kernel_boot_info;

#endif // _BOOT_INFO_H
