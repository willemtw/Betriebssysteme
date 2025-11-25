#include "arch/cpu/modes.h"
#include <lib/kprintf.h>
#include <arch/bsp/uart.h>
#include <arch/cpu/exception_print.h>
#include <tests/regcheck.h>
#include <config.h>
#include <kernel/systick.h>
#include <arch/cpu/interrupts.h>
#include <arch/cpu/registers.h>

static void subprogram [[noreturn]] (void);

void start_kernel [[noreturn]] (void)
{
	setup_interrupts();
	systick_init();

	kprintf("=== Betriebssystem gestartet ===\n");
	// test_kernel();
	while (true) {
		// svc_call(0);
		kprintf("Mode: %s\n", get_mode_name(read_cpsr().d.mode));
		asm volatile("svc 1");
		char c = uart_getc();
		switch (c) {
		case 'd':
			irq_debug = !irq_debug;
			break;
		//case 'a':
		//	do_data_abort();
		//	break;
		//case 'p':
		//	do_prefetch_abort();
		//	break;
		//case 's':
		//	do_supervisor_call();
		//	break;
		//case 'u':
		//	do_undefined_inst();
		//	break;
		case 'c':
			register_checker();
			break;
		case 'e':
			subprogram();
		default:
			kprintf("Unknown input: %c\n", c);
			break;
		}
	}
}

static void subprogram [[noreturn]] (void)
{
	while (true) {
		char c = uart_getc();
		for (unsigned int n = 0; n < PRINT_COUNT; n++) {
			uart_putc(c);
			volatile unsigned int i = 0;
			for (; i < BUSY_WAIT_COUNTER; i++) {
			}
		}
	}
}
