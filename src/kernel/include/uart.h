#pragma once

#include <stdbool.h>
#include <stdint.h>

int
uart_init(void);

bool
uart_is_available(void);

bool
uart_is_recieve_buffer_empty(void);

char
uart_get_char(void);

bool
uart_is_transmit_buffer_empty(void);

void
uart_put_char(char a);

void
uart_puts(const char* str);
