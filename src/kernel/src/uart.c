#include <stdint.h>
#include <stddef.h>
#include <sys/io.h>
#include <string.h>
#include "uart.h"

#define UART_PORT_COM1 0x3f8

void uart_init()
{
    outb(UART_PORT_COM1 + 1, 0x00); // Disable all interrupts
    outb(UART_PORT_COM1 + 3, 0x80); // Enable DLAB (set baud rate divisor)
    outb(UART_PORT_COM1 + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
    outb(UART_PORT_COM1 + 1, 0x00); //                  (hi byte)
    outb(UART_PORT_COM1 + 3, 0x03); // 8 bits, no parity, one stop bit
    outb(UART_PORT_COM1 + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
    outb(UART_PORT_COM1 + 4, 0x0B); // IRQs enabled, RTS/DSR set
}

bool uart_is_recieve_buffer_empty()
{
    return inb(UART_PORT_COM1 + 5) & 1;
}

char uart_getchar()
{
    while (!uart_is_recieve_buffer_empty());
    return inb(UART_PORT_COM1);
}

bool uart_is_transmit_buffer_empty()
{
    return (inb(UART_PORT_COM1 + 5) & 0x20) != 0;
}

void uart_putchar(char a)
{
    while (!uart_is_transmit_buffer_empty());
    outb(UART_PORT_COM1, a);
}

void uart_puts(const char* str)
{
    for(size_t i = 0; i < strlen(str); i++) 
        uart_putchar(str[i]);
}

void _putchar(char c)
{
    uart_putchar(c);
}
