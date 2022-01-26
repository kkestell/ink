#pragma once

#include <stdbool.h>

#define UART_PORT_COM1 0x3f8

void uart_init();
bool uart_is_recieve_buffer_empty();
char uart_getchar();
bool uart_is_transmit_buffer_empty();
void uart_putchar(char a);
void uart_puts(const char* str);
