import
  boot_info,
  framebuffer

proc getBootInfo(): ref KernelBootInfo =
  var info = new KernelBootInfo
  asm """
    movq %%r15,%0
    : "=r" (`info`)
  """
  return info

proc initMemory(memory_map: KernelMemoryMap)
  {.header: "kalloc.h", importc: "kalloc_init".}

proc initUart() 
  {.header: "uart.h", importc: "uart_init".}

proc initConsole(framebuffer: KernelFramebufferInfo) 
  {.header: "console.h", importc: "console_init".}

proc debugMemory*() 
  {.header: "kalloc.h", importc: "kalloc_debug".}

proc puts*(str: cstring) 
  {.header: "console.h", importc: "console_puts".}

proc initPlatform*() =
  initUart()

  let info = getBootInfo()

  initMemory(info.memory)

  initFramebuffer(info.framebuffer)
  initConsole(info.framebuffer)
