import
  boot_info,
  framebuffer

var info: ref KernelBootInfo

proc initMemory(memory_map: KernelMemoryMap)
  {.header: "kalloc.h", importc: "kalloc_init".}

proc debugMemory*() 
  {.header: "kalloc.h", importc: "kalloc_debug".}

proc initUart() 
  {.header: "uart.h", importc: "uart_init".}

proc initConsole(framebuffer: Framebuffer) 
  {.header: "console.h", importc: "console_init".}

proc getBootInfo(): ref KernelBootInfo =
  var info = new KernelBootInfo
  asm """
    movq %%r15,%0
    : "=r" (`info`)
  """
  return info

proc getFramebuffer*(): Framebuffer = info.framebuffer

proc initPlatform*() =
  info = getBootInfo()

  initMemory(info.memory)
  initUart()
  initConsole(info.framebuffer)
