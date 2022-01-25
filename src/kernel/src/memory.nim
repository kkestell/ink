import
    boot_info

proc initMemory*(memory_map: KernelMemoryMap) {.header: "kalloc.h", importc: "kalloc_init"}
