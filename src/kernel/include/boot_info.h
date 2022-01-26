#pragma once

#include <stdint.h>

typedef struct framebuffer_pixel_info
{
    uint32_t red;
    uint32_t green;
    uint32_t blue;
    uint32_t mask;
} framebuffer_pixel_info_t;

typedef enum framebuffer_pixel_format
{
    PixelRedGreenBlueReserved8BitPerColor,
    PixelBlueGreenRedReserved8BitPerColor,
    PixelBitMask,
    PixelBltOnly,
    PixelFormatMax
} framebuffer_pixel_format_t;

typedef struct kernel_framebuffer_info
{
    uint32_t                   framebuffer_width;
    uint32_t                   framebuffer_height;
    framebuffer_pixel_format_t framebuffer_pixel_format;
    framebuffer_pixel_info_t   framebuffer_pixel_info;
    uint32_t                   framebuffer_pixels_per_scan_line;
    uint64_t                   framebuffer_base_address;
    uint64_t                   framebuffer_size;
} kernel_framebuffer_info_t;

typedef struct kernel_memory_map_descriptor
{
    uint32_t type;
    void     *physical_start;
    void     *virtual_start;
    uint64_t page_count;
    uint64_t attributes;
} kernel_memory_map_descriptor_t;

typedef struct kernel_memory_map
{
    kernel_memory_map_descriptor_t *memory_map;
    uint64_t memory_map_size;
    uint64_t key;
    uint64_t memory_map_descriptor_size;
    uint32_t memory_map_descriptor_version;
} kernel_memory_map_t;
