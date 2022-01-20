#ifndef _BOOT_INFO_H
#define _BOOT_INFO_H

#include <stdint.h>

typedef struct pixel_bitmask {
    uint32_t  red;
    uint32_t  green;
    uint32_t  blue;
    uint32_t  reserved;
} pixel_bitmask;

typedef enum pixelFormat {
    PixelRedGreenBlueReserved8BitPerColor,
    PixelBlueGreenRedReserved8BitPerColor,
    PixelBitMask,
    PixelBltOnly,
    PixelFormatMax
} pixelFormat;

typedef struct memory_map_descriptor
{
    uint32_t type;
    uintptr_t physical_start;
    uintptr_t virtual_start;
    uint64_t count;
    uint64_t attributes;
} memory_map_descriptor;

typedef struct kernel_boot_info
{
    // mm
    uint64_t* memory_map;
    uint64_t memoryMapSize;
    uint64_t memoryMapKey;
    uint64_t descriptorSize;
    uint32_t descriptorVersion;

    // fb
    uint32_t horizontalResolution;
    uint32_t verticalResolution;
    pixelFormat pixelFormat;
    pixel_bitmask pixel_bitmask;
    uint32_t pixelsPerScanLine;
    uint64_t framebufferBaseAddress;
    uint64_t framebufferSize;
} kernel_boot_info;

#endif // _BOOT_INFO_H
