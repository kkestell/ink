proc puts*(str: cstring) 
  {.header: "console.h", importc: "console_puts".}
