type
  PixelBitmask* {.bycopy.} = object
    red*: uint32
    green*: uint32
    blue*: uint32
    reserved*: uint32

  PixelFormat* = enum
    PixelFormatRedGreenBlueReserved8BitPerColor, PixelFormatBlueGreenRedReserved8BitPerColor, PixelFormatBitMask, PixelFormatBltOnly, PixelFormatMax

  MemoryMapDescriptor* {.bycopy.} = object
    descriptor_type*: uint32
    physical_start*: pointer
    virtual_start*: pointer
    page_count*: uint64
    attributes*: uint64

  KernelBootInfo* {.bycopy.} = object
    memory_map*: ptr MemoryMapDescriptor
    memory_map_size*: uint64
    memory_map_key*: uint64
    memory_map_descriptor_size*: uint64
    memory_map_descriptor_version*: uint32
    framebuffer_width*: uint32
    framebuffer_height*: uint32
    framebuffer_pixel_format*: PixelFormat
    framebuffer_pixel_bitmask*: PixelBitmask
    framebuffer_pixels_per_scan_line*: uint32
    framebuffer_base_address*: uint64
    framebuffer_size*: uint64