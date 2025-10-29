#ifndef UART_H
#define UART_H

#include <stdarg.h>

void uart_putc(char c);
char uart_getc(void);
void uart_puts(const char *str);

#endif