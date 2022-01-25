import strutils

proc inb*(port: uint16): char {.header: "sys/io.h", importc: "inb"}
proc outb*(port: uint16; val: char) {.header: "sys/io.h", importc: "outb"}

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

proc get_boot_info*(): ptr KernelBootInfo {.header: "boot.h", importc: "get_boot_info"}

const
  UART_PORT_COM1* = 0x3f8'u16

proc uart_init*() =
  outb(UART_PORT_COM1 + 1, char(0x00)) ##  Disable all interrupts
  outb(UART_PORT_COM1 + 3, char(0x80)) ##  Enable DLAB (set baud rate divisor)
  outb(UART_PORT_COM1 + 0, char(0x03)) ##  Set divisor to 3 (lo byte) 38400 baud
  outb(UART_PORT_COM1 + 1, char(0x00)) ##                   (hi byte)
  outb(UART_PORT_COM1 + 3, char(0x03)) ##  8 bits, no parity, one stop bit
  outb(UART_PORT_COM1 + 2, char(0xC7)) ##  Enable FIFO, clear them, with 14-byte threshold
  outb(UART_PORT_COM1 + 4, char(0x0B)) ##  IRQs enabled, RTS/DSR set

proc uart_is_transmit_buffer_empty*(): bool =
  return (int(inb(UART_PORT_COM1 + 5)) and 0x20) != 0

proc uart_putchar*(a: char) =
  while not uart_is_transmit_buffer_empty():
    discard
  outb(UART_PORT_COM1, a)

proc uart_puts*(str: cstring) =
  for c in items(str):
    uart_putchar(c)

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

uart_init()
let info = get_boot_info()

clear(info)

uart_puts("memory_map_size:                  ")
uart_puts(intToStr(cast[int](info.memory_map_size)).cstring)
uart_puts("\n")
uart_puts("memory_map_key:                   ")
uart_puts(intToStr(cast[int](info.memory_map_key)).cstring)
uart_puts("\n")
uart_puts("memory_map_descriptor_size:       ")
uart_puts(intToStr(cast[int](info.memory_map_descriptor_size)).cstring)
uart_puts("\n")
uart_puts("memory_map_descriptor_version:    ")
uart_puts(intToStr(cast[int](info.memory_map_descriptor_version)).cstring)
uart_puts("\n")
uart_puts("framebuffer_width:                ")
uart_puts(intToStr(cast[int](info.framebuffer_width)).cstring)
uart_puts("\n")
uart_puts("framebuffer_height:               ")
uart_puts(intToStr(cast[int](info.framebuffer_height)).cstring)
uart_puts("\n")
uart_puts("framebuffer_pixels_per_scan_line: ")
uart_puts(intToStr(cast[int](info.framebuffer_pixels_per_scan_line)).cstring)
uart_puts("\n")
uart_puts("framebuffer_base_address:         ")
uart_puts(intToStr(cast[int](info.framebuffer_base_address)).cstring)
uart_puts("\n")
uart_puts("framebuffer_size:                 ")
uart_puts(intToStr(cast[int](info.framebuffer_size)).cstring)
uart_puts("\n")
