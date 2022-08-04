const
  UartPortCom1* = 0x3f8

proc inb*(port: uint16): uint8 =
  var ret: uint8
  asm """
    inb %1, %0
    :"=a"(`ret`)
    :"Nd"(`port`)
  """
  return ret

proc outb*(port: uint16; val: uint8) =
  asm """
    outb %0, %1
    :
    :"a"(`val`), "Nd"(`port`)
  """

proc uartInit*() =
  outb(UartPortCom1.uint16 + 1, 0x00) ##  Disable all interrupts
  outb(UartPortCom1.uint16 + 3, 0x80) ##  Enable DLAB (set baud rate divisor)
  outb(UartPortCom1.uint16 + 0, 0x03) ##  Set divisor to 3 (lo byte) 38400 baud
  outb(UartPortCom1.uint16 + 1, 0x00) ##                   (hi byte)
  outb(UartPortCom1.uint16 + 3, 0x03) ##  8 bits, no parity, one stop bit
  outb(UartPortCom1.uint16 + 2, 0xC7) ##  Enable FIFO, clear them, with 14-byte threshold
  outb(UartPortCom1.uint16 + 4, 0x0B) ##  IRQs enabled, RTS/DSR set

proc uart_is_recieve_buffer_empty(): bool {.exportc.} =
  return (inb(UartPortCom1 + 5) and 1) == 1

proc uart_get_char(): char {.exportc.} =
  while not uart_is_recieve_buffer_empty():
    discard
  return cast[char](inb(UartPortCom1))

proc uart_is_transmit_buffer_empty(): bool {.exportc.} =
  return (inb(UartPortCom1 + 5) and 0x20) != 0

proc uart_put_char(a: char) {.exportc.} =
  while not uart_is_transmit_buffer_empty():
    discard
  outb(UartPortCom1, cast[uint8](a))

proc uart_puts(str: string) {.exportc.} =
  for c in str:
    uartPutChar(c)
