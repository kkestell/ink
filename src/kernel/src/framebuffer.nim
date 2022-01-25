import boot_info

proc put_pixel*(boot_info: ptr KernelBootInfo; x: uint32; y: uint32; c: uint32) =
  (cast[ptr uint32]((boot_info.framebuffer_base_address +
      4 * boot_info.framebuffer_pixels_per_scan_line * y + 4 * x)))[] = c

proc clear*(boot_info: ptr KernelBootInfo) =
  var c: uint32 = 0x3b6ea5
  var y: uint32 = 0
  while y < boot_info.framebuffer_height:
    var x: uint32 = 0
    while x < boot_info.framebuffer_width:
      put_pixel(boot_info, x, y, c)
      inc(x)
    inc(y)