#ifndef _BOOT_INFO_H
#define _BOOT_INFO_H

#include "uefi.h"

typedef struct KernelBootInfo
{
    // mm
    UEFI_MEMORY_DESCRIPTOR* memory_map;
    uint64_t memoryMapSize;
    uint64_t memoryMapKey;
    uint64_t descriptorSize;
    uint32_t descriptorVersion;

    // fb
    uint32_t horizontalResolution;
    uint32_t verticalResolution;
    UEFI_GRAPHICS_PIXEL_FORMAT pixelFormat;
    UEFI_PIXEL_BITMASK pixelInformation;
    uint32_t pixelsPerScanLine;
    uint64_t framebufferBaseAddress;
    uint64_t framebufferSize;
} KernelBootInfo;

#endif // _BOOT_INFO_H
