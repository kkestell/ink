import boot_info

var fb: KernelFramebufferInfo

proc initFramebuffer*(framebuffer_info: KernelFramebufferInfo) =
  fb = framebuffer_info

proc putPixel*(x: uint32; y: uint32; c: uint32) =
  (cast[ptr uint32]((fb.baseAddress + 4 * fb.pixelsPerScanLine * y + 4 * x)))[] = c

proc clearScreen*(c: uint32 = 0x3b6ea5) =
  for y in countup(0'u32, fb.height):
    for x in countup(0'u32, fb.width):
      putPixel(x, y, c)
