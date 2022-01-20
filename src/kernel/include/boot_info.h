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
    uint32_t type;
    uintptr_t physicalStart;
    uintptr_t virtualStart;
    uint64_t count;
    uint64_t attributes;
} MemoryMapDescriptor;

typedef struct KernelBootInfo
{
    // mm
    MemoryMapDescriptor* memoryMap;
    uint64_t memoryMapSize;
    uint64_t memoryMapKey;
    uint64_t descriptorSize;
    uint32_t descriptorVersion;

    // fb
    uint32_t horizontalResolution;
    uint32_t verticalResolution;
    PixelFormat pixelFormat;
    PixelBitmask pixelBitmask;
    uint32_t pixelsPerScanLine;
    uint64_t framebufferBaseAddress;
    uint64_t framebufferSize;
} KernelBootInfo;

#endif // _BOOT_INFO_H
