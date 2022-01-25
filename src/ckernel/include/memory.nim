import
  boot_info

proc initialize_memory*(boot_info: ptr KernelBootInfo)
proc malloc*(): pointer
proc free*(memory: pointer)
proc print_buddy*()