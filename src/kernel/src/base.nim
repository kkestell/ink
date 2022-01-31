proc printf* (fmt: cstring) {.importc: "kprintf", varargs.}

proc `+`*(a: pointer, n: uint64): pointer =
  cast[pointer](cast[uint64](a) + n)
