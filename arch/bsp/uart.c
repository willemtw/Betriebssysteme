//#include <arch/bsp/uart.h>
#include <stddef.h>
#include <stdarg.h>

static constexpr unsigned int UART_BASE = 0x7E201000 - 0x3F000000;

struct uart {
	unsigned int dr;
	unsigned int unused0[5];
	unsigned int fr;
	unsigned int unused1;
	unsigned int ilpr;
	unsigned int ibrd;
	unsigned int fbrd;
	unsigned int lcrh;
	unsigned int cr;
	unsigned int ifls;
	unsigned int imsc;
};

volatile struct uart *UART = (volatile struct uart *)UART_BASE;

#define UART_FR_TXFF (1 << 5)
#define UART_FR_RXFE (1 << 4)

static_assert(offsetof(struct uart, dr) == 0x00);
static_assert(offsetof(struct uart, fr) == 0x18);
static_assert(offsetof(struct uart, cr) == 0x30);

char uart_getc(void)
{
	while (UART->fr & UART_FR_RXFE);
	return UART->dr;
}

void uart_putc(char c)
{
	while (UART->fr & UART_FR_TXFF);
	UART->dr = c;
}

void uart_puts(const char *str)
{
	while (*str) {
		uart_putc(*str++);
	}
}