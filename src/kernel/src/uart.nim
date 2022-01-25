import
  io

const
  UART_PORT_COM1* = 0x3f8'u16

proc initUart*() =
  outb(UART_PORT_COM1 + 1, char(0x00)) #  Disable all interrupts
  outb(UART_PORT_COM1 + 3, char(0x80)) #  Enable DLAB (set baud rate divisor)
  outb(UART_PORT_COM1 + 0, char(0x03)) #  Set divisor to 3 (lo byte) 38400 baud
  outb(UART_PORT_COM1 + 1, char(0x00)) #                   (hi byte)
  outb(UART_PORT_COM1 + 3, char(0x03)) #  8 bits, no parity, one stop bit
  outb(UART_PORT_COM1 + 2, char(0xC7)) #  Enable FIFO, clear them, with 14-byte threshold
  outb(UART_PORT_COM1 + 4, char(0x0B)) #  IRQs enabled, RTS/DSR set

proc isTransmitBufferEmpty*(): bool =
  return (int(inb(UART_PORT_COM1 + 5)) and 0x20) != 0

proc putchar*(a: char) =
  while not isTransmitBufferEmpty():
    discard
  outb(UART_PORT_COM1, a)

proc puts*(str: cstring) =
  for c in items(str):
    putchar(c)