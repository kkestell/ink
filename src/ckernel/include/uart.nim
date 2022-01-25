proc uart_init*()
proc uart_is_recieve_buffer_empty*(): bool
proc uart_getchar*(): char
proc uart_is_transmit_buffer_empty*(): bool
proc uart_putchar*(a: char)
proc uart_puts*(str: cstring)