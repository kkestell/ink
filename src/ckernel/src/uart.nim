import
  string

const
  UART_PORT_COM1* = 0x3f8

proc uart_init*() =
  outb(UART_PORT_COM1 + 1, 0x00)
  ##  Disable all interrupts
  outb(UART_PORT_COM1 + 3, 0x80)
  ##  Enable DLAB (set baud rate divisor)
  outb(UART_PORT_COM1 + 0, 0x03)
  ##  Set divisor to 3 (lo byte) 38400 baud
  outb(UART_PORT_COM1 + 1, 0x00)
  ##                   (hi byte)
  outb(UART_PORT_COM1 + 3, 0x03)
  ##  8 bits, no parity, one stop bit
  outb(UART_PORT_COM1 + 2, 0xC7)
  ##  Enable FIFO, clear them, with 14-byte threshold
  outb(UART_PORT_COM1 + 4, 0x0B)
  ##  IRQs enabled, RTS/DSR set

proc uart_is_recieve_buffer_empty*(): bool =
  return inb(UART_PORT_COM1 + 5) and 1

proc uart_getchar*(): char =
  while not uart_is_recieve_buffer_empty():
    ## ignored statement
  return inb(UART_PORT_COM1)

proc uart_is_transmit_buffer_empty*(): bool =
  return (inb(UART_PORT_COM1 + 5) and 0x20) != 0

proc uart_putchar*(a: char) =
  while not uart_is_transmit_buffer_empty():
    ## ignored statement
  outb(UART_PORT_COM1, a)

proc uart_puts*(str: cstring) =
  var i: csize_t = 0
  while i < strlen(str):
    uart_putchar(str[i])
    inc(i)

proc _putchar*(c: char) =
  uart_putchar(c)
