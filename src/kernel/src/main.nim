import strutils
import boot_info

proc uart_init*() {.header: "uart.h", importc: "uart_init"}
proc uart_puts*(str: cstring) {.header: "uart.h", importc: "uart_puts"}
uart_init()

proc get_boot_info*(): ptr KernelBootInfo {.header: "boot.h", importc: "get_boot_info"}
let info = get_boot_info()

proc kalloc_init*(memory_map: pointer, memory_map_size: uint64, memory_map_descriptor_size: uint64) {.header: "kalloc.h", importc: "kalloc_init"}
proc kalloc_debug*() {.header: "kalloc.h", importc: "kalloc_debug"}
kalloc_init(info.memory_map, info.memory_map_size, info.memory_map_descriptor_size)

proc foo() =
  var ints: seq[int]

  for i in countup(1, 1000000):
    ints.add(100)

foo()


#for i in ints:
#  uart_puts(intToStr(i).cstring)

#kalloc_debug()

# uart_puts("memory_map_size:                  ")
# uart_puts(intToStr(cast[int](info.memory_map_size)).cstring)
# uart_puts("\n")
# uart_puts("memory_map_key:                   ")
# uart_puts(intToStr(cast[int](info.memory_map_key)).cstring)
# uart_puts("\n")
# uart_puts("memory_map_descriptor_size:       ")
# uart_puts(intToStr(cast[int](info.memory_map_descriptor_size)).cstring)
# uart_puts("\n")
# uart_puts("memory_map_descriptor_version:    ")
# uart_puts(intToStr(cast[int](info.memory_map_descriptor_version)).cstring)
# uart_puts("\n")
# uart_puts("framebuffer_width:                ")
# uart_puts(intToStr(cast[int](info.framebuffer_width)).cstring)
# uart_puts("\n")
# uart_puts("framebuffer_height:               ")
# uart_puts(intToStr(cast[int](info.framebuffer_height)).cstring)
# uart_puts("\n")
# uart_puts("framebuffer_pixels_per_scan_line: ")
# uart_puts(intToStr(cast[int](info.framebuffer_pixels_per_scan_line)).cstring)
# uart_puts("\n")
# uart_puts("framebuffer_base_address:         ")
# uart_puts(intToStr(cast[int](info.framebuffer_base_address)).cstring)
# uart_puts("\n")
# uart_puts("framebuffer_size:                 ")
# uart_puts(intToStr(cast[int](info.framebuffer_size)).cstring)
uart_puts("\n")
