type
  PixelBitmask* = object
    red*: uint32
    green*: uint32
    blue*: uint32
    reserved*: uint32

  PixelFormat* = enum
    pfRedGreenBlueReserved8BitPerColor, pfBlueGreenRedReserved8BitPerColor,
        pfBitMask, pfBltOnly, pfMax

  Framebuffer* = object
    width*: uint32
    height*: uint32
    pixelFormat*: PixelFormat
    pixelBitmask*: PixelBitmask
    pixelsPerScanLine*: uint32
    baseAddress*: uint64
    size*: uint64

func putPixel*(f: Framebuffer, x: uint32; y: uint32; c: uint32) =
  (cast[ptr uint32]((f.baseAddress + 4 * f.pixelsPerScanLine * y + 4 * x)))[] = c

func clearScreen*(f: Framebuffer, c: uint32 = 0x000000) =
  for y in countup(0'u32, f.height):
    for x in countup(0'u32, f.width):
      f.putPixel(x, y, c)
