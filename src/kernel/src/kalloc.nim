import
  base,
  boot

const
  EfiMaxMemoryType* = 0x0000000e
  EfiConventionalMemory* = 0x00000007

var head: pointer

proc kallocInit* (mm: var BootMemoryMap) =
  printf("MEMORY MAP\n")
  printf("========================================\n")
  printf("map:                %p\n", cast[uint64](mm.map))
  printf("size:               %U\n", mm.size)
  printf("descriptorSize:     %U\n", mm.descriptorSize)
  printf("descriptorVersion:  %u\n", mm.descriptorVersion)
  printf("----------------------------------------\n")

  var d: ptr MemoryMapDescriptor = cast[ptr MemoryMapDescriptor](mm.map)
  var start: pointer = nil
  var pageCount: uint64 = 0

  for i in 0..mm.size div mm.descriptorSize:
    d = cast[ptr MemoryMapDescriptor](cast[pointer](cast[int](mm.map) + 
      cast[int](i * mm.descriptorSize)))

    if d.descriptorType == EfiConventionalMemory:
      printf("descriptorType: %u\n", d.descriptorType)
      printf("physicalStart:  %p\n", cast[uint64](d.physicalStart))
      printf("virtualStart:   %p\n", cast[uint64](d.virtualStart))
      printf("pageCount:      %U\n", d.pageCount)
      printf("attributes:     %U\n", d.attributes)
      printf("----------------------------------------\n")

      if pageCount < d.pageCount:
        start = cast[pointer](d.physicalStart)
        pageCount = d.pageCount

  head = start

  printf("kallocInit: mapped %U pages at %p\n", pageCount, cast[uint64](start))

proc malloc(size: uint64): pointer {.exportc.} =
  let p: pointer = head
  printf("malloc: mapping %U bytes at %p\n", size, cast[uint64](p))
  head = head + size
  return p

proc free(p: pointer) {.exportc.} =
  printf("free: %p\n", cast[uint64](p))
  discard

proc calloc(number: uint64, size: uint64): pointer {.exportc.} =
  let p: pointer = head
  printf("calloc: mapping %U bytes at %p\n", number * size, cast[uint64](p))
  head = head + (number * size)
  return p

proc realloc(p: pointer, size: uint64): pointer {.exportc.} =
  let n: pointer = head
  copyMem(n, p, size)
  printf("realloc: mapping %p to %U bytes at %p\n", cast[uint64](p), size, cast[uint64](n))
  head = head + size
  return n
