#include <arch/bsp/uart.h>

char uart_getc(void)
{
	while (UART->fr & UART_FR_RXFE)
		;
	return UART->dr;
}

void uart_putc(char c)
{
	while (UART->fr & UART_FR_TXFF)
		;
	UART->dr = c;
}

void uart_puts(const char *str)
{
	while (*str) {
		uart_putc(*str++);
	}
}
