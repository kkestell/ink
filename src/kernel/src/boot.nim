type
  # PixelBitmask* = object
  #   red*: uint32
  #   green*: uint32
  #   blue*: uint32
  #   reserved*: uint32

  # PixelFormat* = enum
  #   pfRedGreenBlueReserved8BitPerColor, pfBlueGreenRedReserved8BitPerColor,
  #       pfBitMask, pfBltOnly, pfMax

  # BootFramebuffer* = object
  #   width*: uint32
  #   height*: uint32
  #   pixelFormat*: PixelFormat
  #   pixelBitmask*: PixelBitmask
  #   pixelsPerScanLine*: uint32
  #   baseAddress*: uint64
  #   size*: uint64

  MemoryMapDescriptor* = object
    descriptorType*: uint32
    physicalStart*: pointer
    virtualStart*: pointer
    pageCount*: uint64
    attributes*: uint64

  BootMemoryMap* = object
    map*: pointer
    size*: uint64
    key*: uint64
    descriptorSize*: uint64
    descriptorVersion*: uint32

  BootConfig* = object
    memory*: BootMemoryMap
    # framebuffer*: BootFramebuffer

var cfg: ptr BootConfig = nil

proc getBootConfig*(): ptr BootConfig =
  if cfg == nil:
    var foo: pointer
    asm """
      movq %%r15,%0
      : "=r" (`foo`)
    """

    cfg = cast[ptr BootConfig](foo)
    return cfg
