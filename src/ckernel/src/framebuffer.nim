import
  framebuffer

proc put_pixel*(boot_info: ptr KernelBootInfo; x: uint32_t; y: uint32_t; c: uint32_t) =
  (cast[ptr uint32_t]((boot_info.framebuffer_base_address +
      4 * boot_info.framebuffer_pixels_per_scan_line * y + 4 * x)))[] = c

proc hsv_to_rgb*(hsv: HsvColor): RgbColor =
  var rgb: RgbColor
  var
    region: cuchar
    remainder: cuchar
    p: cuchar
    q: cuchar
    t: cuchar
  if hsv.s == 0:
    rgb.r = hsv.v
    rgb.g = hsv.v
    rgb.b = hsv.v
    return rgb
  region = hsv.h div 43
  remainder = (hsv.h - (region * 43)) * 6
  p = (hsv.v * (255 - hsv.s)) shr 8
  q = (hsv.v * (255 - ((hsv.s * remainder) shr 8))) shr 8
  t = (hsv.v * (255 - ((hsv.s * (255 - remainder)) shr 8))) shr 8
  case region
  of 0:
    rgb.r = hsv.v
    rgb.g = t
    rgb.b = p
  of 1:
    rgb.r = q
    rgb.g = hsv.v
    rgb.b = p
  of 2:
    rgb.r = p
    rgb.g = hsv.v
    rgb.b = t
  of 3:
    rgb.r = p
    rgb.g = q
    rgb.b = hsv.v
  of 4:
    rgb.r = t
    rgb.g = p
    rgb.b = hsv.v
  else:
    rgb.r = hsv.v
    rgb.g = p
    rgb.b = q
  return rgb

proc rgb_to_hsv*(rgb: RgbColor): HsvColor =
  var hsv: HsvColor
  var
    rgbMin: cuchar
    rgbMax: cuchar
  rgbMin = if rgb.r < rgb.g: (if rgb.r < rgb.b: rgb.r else: rgb.b) else: (
      if rgb.g < rgb.b: rgb.g else: rgb.b)
  rgbMax = if rgb.r > rgb.g: (if rgb.r > rgb.b: rgb.r else: rgb.b) else: (
      if rgb.g > rgb.b: rgb.g else: rgb.b)
  hsv.v = rgbMax
  if hsv.v == 0:
    hsv.h = 0
    hsv.s = 0
    return hsv
  hsv.s = 255 * (long)(rgbMax - rgbMin) div hsv.v
  if hsv.s == 0:
    hsv.h = 0
    return hsv
  if rgbMax == rgb.r:
    hsv.h = 0 + 43 * (rgb.g - rgb.b) div (rgbMax - rgbMin)
  elif rgbMax == rgb.g:
    hsv.h = 85 + 43 * (rgb.b - rgb.r) div (rgbMax - rgbMin)
  else:
    hsv.h = 171 + 43 * (rgb.r - rgb.g) div (rgbMax - rgbMin)
  return hsv

proc rgb_to_color*(rgb: RgbColor): uint32_t =
  var color: uint32_t = rgb.r
  color = (color shl 8) + rgb.g
  color = (color shl 8) + rgb.b
  return color

proc julia*(boot_info: ptr KernelBootInfo) =
  var w: int32_t = boot_info.framebuffer_width
  var h: int32_t = boot_info.framebuffer_height
  var
    cRe: cdouble
    cIm: cdouble
  var
    newRe: cdouble
    newIm: cdouble
    oldRe: cdouble
    oldIm: cdouble
  var
    zoom: cdouble = 1
    moveX: cdouble = 0
    moveY: cdouble = 0
  var maxIterations: int32_t = 300
  cRe = -0.7
  cIm = 0.27015
  var y: int32_t = 0
  while y < h:
    var x: int32_t = 0
    while x < w:
      newRe = 1.5 * (x - w div 2) div (0.5 * zoom * w) + moveX
      newIm = (y - h div 2) div (0.5 * zoom * h) + moveY
      var i: int32_t
      i = 0
      while i < maxIterations:
        oldRe = newRe
        oldIm = newIm
        newRe = oldRe * oldRe - oldIm * oldIm + cRe
        newIm = 2 * oldRe * oldIm + cIm
        if (newRe * newRe + newIm * newIm) > 4:
          break
        inc(i)
      var c: HsvColor
      c.h = i mod 256
      c.s = 255
      c.v = 255 * (i < maxIterations)
      put_pixel(boot_info, x, y, rgb_to_color(hsv_to_rgb(c)))
      inc(x)
    inc(y)

proc clear*(boot_info: ptr KernelBootInfo) =
  var c: uint32_t = 0x3b6ea5
  var y: uint32_t = 0
  while y < boot_info.framebuffer_height:
    var x: uint32_t = 0
    while x < boot_info.framebuffer_width:
      put_pixel(boot_info, x, y, c)
      inc(x)
    inc(y)
