#ifndef UART_H
#define UART_H
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

enum uart_irq {
	UART_RX = 4,
};

struct uart_fr {
	union {
		uint32_t r;

		struct {
			uint8_t	 cts : 1;
			uint8_t	 dsr : 1;
			uint8_t	 dcd : 1;
			uint8_t	 busy : 1;
			uint8_t	 rxfe : 1;
			uint8_t	 txff : 1;
			uint8_t	 rxff : 1;
			uint8_t	 txfe : 1;
			uint8_t	 ri : 1;
			uint32_t _reserved : 23;
		} d;
	};
};

struct uart_irq_bits {
	union {
		uint32_t r;

		struct {
			uint8_t	 ri : 1;
			uint8_t	 cts : 1;
			uint8_t	 dcd : 1;
			uint8_t	 dsr : 1;
			uint8_t	 rx : 1;
			uint8_t	 tx : 1;
			uint8_t	 rt : 1;
			uint8_t	 fe : 1;
			uint8_t	 pe : 1;
			uint8_t	 be : 1;
			uint8_t	 oe : 1;
			uint32_t _reserved : 21;
		} d;
	};
};

static_assert(sizeof(struct uart_fr) == 4);
static_assert(sizeof(struct uart_irq_bits) == 4);

struct uart {
	uint32_t	     dr;
	uint32_t	     unused0[5];
	struct uart_fr	     fr;
	uint32_t	     unused1;
	uint32_t	     ilpr;
	uint32_t	     ibrd;
	uint32_t	     fbrd;
	uint32_t	     lcrh;
	uint32_t	     cr;
	uint32_t	     ifls;
	struct uart_irq_bits imsc;
	struct uart_irq_bits ris;
	struct uart_irq_bits mis;
	struct uart_irq_bits icr;
};

#define UART_BASE (0x7E201000 - 0x3F000000)

#define UART ((volatile struct uart *)UART_BASE)

#define UART_FR_TXFF (1 << 5)
#define UART_FR_RXFE (1 << 4)

static_assert(offsetof(struct uart, dr) == 0x00);
static_assert(offsetof(struct uart, fr) == 0x18);
static_assert(offsetof(struct uart, cr) == 0x30);

void uart_putc(char c);
char uart_getc(void);
void uart_puts(const char *str);

void uart_enable_irq(enum uart_irq irq);
void uart_disable_irq(enum uart_irq irq);
bool uart_check_irq(enum uart_irq irq);
void uart_clear_irq(enum uart_irq irq);

void uart_handle_irq(void);

void uart_init(void);

#endif
