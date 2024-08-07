#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <port_io.h>
#include "string.h"

#define UART_PORT_COM1 0x3f8

static bool uart_available = false;

int uart_init(void)
{
    outb(UART_PORT_COM1 + 1, 0x00); // Disable all interrupts
    outb(UART_PORT_COM1 + 3, 0x80); // Enable DLAB (set baud rate divisor)
    outb(UART_PORT_COM1 + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
    outb(UART_PORT_COM1 + 1, 0x00); //                  (hi byte)
    outb(UART_PORT_COM1 + 3, 0x03); // 8 bits, no parity, one stop bit
    outb(UART_PORT_COM1 + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
    outb(UART_PORT_COM1 + 4, 0x0B); // IRQs enabled, RTS/DSR set
    outb(UART_PORT_COM1 + 4, 0x1E); // Set in loopback mode, test the serial chip
    outb(UART_PORT_COM1 + 0, 0xAE); // Test serial chip (send byte 0xAE and check if serial returns same byte)

   // Check if serial is faulty (i.e: not same byte as sent)
   if(inb(UART_PORT_COM1 + 0) != 0xAE) {
        uart_available = false;
        return 1;
   }

   // If serial is not faulty set it in normal operation mode
   // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
   outb(UART_PORT_COM1 + 4, 0x0F);
   uart_available = true;
   return 0;
}

bool uart_is_available(void)
{
    return uart_available;
}

bool uart_is_recieve_buffer_empty(void)
{
    return inb(UART_PORT_COM1 + 5) & 1;
}

char uart_get_char(void)
{
    while (!uart_is_recieve_buffer_empty());
    return inb(UART_PORT_COM1);
}

bool uart_is_transmit_buffer_empty(void)
{
    return (inb(UART_PORT_COM1 + 5) & 0x20) != 0;
}

void uart_put_char(char a)
{
    while (!uart_is_transmit_buffer_empty());
    outb(UART_PORT_COM1, a);
}

void uart_puts(const char* str)
{
    for(size_t i = 0; i < strlen(str); i++) 
        uart_put_char(str[i]);
}
