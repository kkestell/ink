import 
  framebuffer

type
  KernelMemoryMap* = object
    map*: pointer
    size*: uint64
    key*: uint64
    descriptorSize*: uint64
    descriptorVersion*: uint32

  KernelBootInfo* = object
    memory*: KernelMemoryMap
    framebuffer*: Framebuffer
