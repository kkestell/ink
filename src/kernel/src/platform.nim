import
  boot_info

proc initMemory*(memory_map: KernelMemoryMap) {.header: "kalloc.h",
    importc: "kalloc_init".}

proc initUart*() {.header: "uart.h", importc: "uart_init".}

proc debugMemory*() {.header: "kalloc.h", importc: "kalloc_debug".}
