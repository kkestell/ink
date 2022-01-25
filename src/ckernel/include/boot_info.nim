import
  types

type
  PixelBitmask* {.bycopy.} = object
    red*: u32
    green*: u32
    blue*: u32
    reserved*: u32

  PixelFormat* = enum
    PixelRedGreenBlueReserved8BitPerColor, PixelBlueGreenRedReserved8BitPerColor,
    PixelBitMask, PixelBltOnly, PixelFormatMax
  MemoryMapDescriptor* {.bycopy.} = object
    `type`*: u32
    physical_start*: pointer
    virtual_start*: pointer
    page_count*: u64
    attributes*: u64

  KernelBootInfo* {.bycopy.} = object
    memory_map*: ptr MemoryMapDescriptor
    memory_map_size*: u64
    memory_map_key*: u64
    memory_map_descriptor_size*: u64
    memory_map_descriptor_version*: u32
    framebuffer_width*: u32
    framebuffer_height*: u32
    framebuffer_pixel_format*: PixelFormat
    framebuffer_pixel_bitmask*: PixelBitmask
    framebuffer_pixels_per_scan_line*: u32
    framebuffer_base_address*: u64
    framebuffer_size*: u64


