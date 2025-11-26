#include <arch/bsp/interrupt_controller.h>
#include <config.h>
#include "lib/ringbuffer.h"
#include <arch/bsp/uart.h>
#include <lib/kprintf.h>

create_ringbuffer(uart_ringbuffer, UART_INPUT_BUFFER_SIZE);

void uart_init(void)
{
	interrupt_controller_enable_irq(UART_IRQ);
	uart_enable_irq(UART_RX);
}

void uart_handle_irq(void)
{
	if (!uart_check_irq(UART_RX)) {
		return;
	}

	while (!UART->fr.d.rxfe)
		buff_putc(uart_ringbuffer, UART->dr);
	uart_clear_irq(UART_RX);
}

void uart_enable_irq(enum uart_irq irq)
{
	UART->imsc.r |= (1 << irq);
}

void uart_disable_irq(enum uart_irq irq)
{
	UART->imsc.r &= ~(1 << irq);
}

bool uart_check_irq(enum uart_irq irq)
{
	return UART->mis.r & (1 << irq);
}

void uart_clear_irq(enum uart_irq irq)
{
	UART->icr.r = (1 << irq);
}

char uart_getc(void)
{
	while (buff_is_empty(uart_ringbuffer))
		;
	return buff_getc(uart_ringbuffer);
}

void uart_putc(char c)
{
	while (UART->fr.d.txff)
		;
	UART->dr = c;
}

void uart_puts(const char *str)
{
	while (*str) {
		uart_putc(*str++);
	}
}
