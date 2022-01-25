type
  PixelBitmask* = object
    red*: uint32
    green*: uint32
    blue*: uint32
    reserved*: uint32

  PixelFormat* = enum
    pfRedGreenBlueReserved8BitPerColor, pfBlueGreenRedReserved8BitPerColor, pfBitMask, pfBltOnly, pfMax

  KernelMemoryMap* = object
    map*: pointer
    size*: uint64
    key*: uint64
    descriptorSize*: uint64
    descriptorVersion*: uint32

  KernelFramebufferInfo* = object
    width*: uint32
    height*: uint32
    pixelFormat*: PixelFormat
    pixelBitmask*: PixelBitmask
    pixelsPerScanLine*: uint32
    baseAddress*: uint64
    size*: uint64

  KernelBootInfo* = object
    memory*: KernelMemoryMap
    framebuffer*: KernelFramebufferInfo

proc getBootInfo*(): ref KernelBootInfo =
  var info = new KernelBootInfo
  asm """
    movq %%r15,%0
    : "=r" (`info`)
  """
  return info
