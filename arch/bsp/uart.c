#include <kernel/threading/scheduler.h>
#include <arch/bsp/interrupt_controller.h>
#include <config.h>
#include <lib/ringbuffer.h>
#include <arch/bsp/uart.h>
#include <lib/kprintf.h>
#include <user/main.h>
#include <tests/exceptions.h>
#include <lib/list.h>

list_create(uart_wait_queue);
create_ringbuffer(uart_ringbuffer, UART_INPUT_BUFFER_SIZE);

void uart_init(void)
{
	// Should already reset to 0, but test said we may be buffering more
	// than the intended ringbuffer size. Since the size is correct, there
	// might be FIFO shenanigans going on.
	UART->lcrh.d.fen = 0;

	interrupt_controller_enable_irq(UART_IRQ);
	uart_enable_irq(UART_RX);
}

void uart_handle_irq(void)
{
	if (!uart_check_irq(UART_RX)) {
		return;
	}

	while (!UART->fr.d.rxfe) {
		char c = UART->dr;
		// First, we try to wake a thread waiting for a character
		if (scheduler_notify_one_from_irq(uart_wait_queue, (uint32_t)c) != RESULT_OK) {
			// kprintf("no thread waiting\n");
			// If no thread was waiting, store the char in the ringbuffer
			buff_putc(uart_ringbuffer, c);
		}
	}
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
	// TODO: Yield/wait syscalls once not all of them need to terminate
	while (buff_is_empty(uart_ringbuffer))
		;
	return buff_getc(uart_ringbuffer);
}

void uart_user_getc(void)
{
	if (buff_is_empty(uart_ringbuffer)) {
		scheduler_wait_running_from_irq(uart_wait_queue);
		return;
	}

	scheduler_prepare_svc_return(buff_getc(uart_ringbuffer));
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
