#ifndef _UART_H
#define _UART_H

#include <stdbool.h>

void uart_init();
bool uart_is_recieve_buffer_empty();
char uart_getchar();
bool uart_is_transmit_buffer_empty();
void uart_putchar(char a);
void uart_puts(const char* str);

#endif // _UART_H
