#pragma once

#include <stdint.h>

typedef struct pixel_bitmask {
    uint32_t red;
    uint32_t green;
    uint32_t blue;
    uint32_t reserved;
} pixel_bitmask_t;

typedef enum pixel_format {
    PixelRedGreenBlueReserved8BitPerColor,
    PixelBlueGreenRedReserved8BitPerColor,
    PixelBitMask,
    PixelBltOnly,
    PixelFormatMax
} pixel_format_t;

typedef struct memory_map_descriptor
{
    uint32_t  type;
    uintptr_t physical_start;
    uintptr_t virtual_start;
    uint64_t  page_count;
    uint64_t  attributes;
} memory_map_descriptor_t;

typedef struct framebuffer_info
{
    uint32_t        width;
    uint32_t        height;
    pixel_format_t  pixel_format;
    pixel_bitmask_t pixel_information;
    uint32_t        pixels_per_scan_line;
    uint64_t        base_address;
    uint64_t        size;
} framebuffer_info_t;

typedef struct memory_info
{
    memory_map_descriptor_t *memory_map;
    uint64_t                 size;
    uint64_t                 key;
    uint64_t                 descriptor_size;
    uint32_t                 descriptor_version;
} memory_info_t;

typedef struct boot_info
{
    memory_info_t      memory_map;
    framebuffer_info_t framebuffer;
} boot_info_t;
