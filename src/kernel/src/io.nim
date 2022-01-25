proc inb*(port: uint16): char =
  asm """
    inb %1, %0
    : "=a" (`result`)
    : "Nd" (`port`)
  """

proc outb*(port: uint16; val: char) =
  asm """
    outb %0, %1
    :
    : "a" (`val`), "Nd" (`port`)
  """

