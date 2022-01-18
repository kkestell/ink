#ifndef _UART_H
#define _UART_H

#include <stdbool.h>

void uart_init(void);
bool uart_is_recieve_buffer_empty(void);
char uart_getchar(void);
bool uart_is_transmit_buffer_empty(void);
void uart_putchar(char a);
void uart_puts(const char* str);

#endif // _UART_H
