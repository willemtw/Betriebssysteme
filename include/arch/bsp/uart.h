#ifndef UART_H
#define UART_H
#include <stddef.h>
#include <stdint.h>

struct uart_t {
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

#define UART_BASE (uint32_t)(0x7E201000 - 0x3F000000)

#define UART ((volatile struct uart_t *)UART_BASE)

#define UART_FR_TXFF (1 << 5)
#define UART_FR_RXFE (1 << 4)

static_assert(offsetof(struct uart_t, dr) == 0x00);
static_assert(offsetof(struct uart_t, fr) == 0x18);
static_assert(offsetof(struct uart_t, cr) == 0x30);

void uart_putc(char c);
char uart_getc(void);
void uart_puts(const char *str);

#endif
