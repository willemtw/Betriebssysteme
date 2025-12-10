#include <kernel/threads/scheduler.h>
#include <lib/kprintf.h>
#include <arch/bsp/uart.h>
#include <arch/cpu/exception_print.h>
#include <tests/regcheck.h>
#include <config.h>
#include <kernel/systick.h>
#include <arch/cpu/interrupts.h>
#include <arch/cpu/registers.h>
#include <tests/exceptions.h>
#include <user/main.h>

void start_kernel [[noreturn]] (void)
{
	setup_interrupts();
	uart_init();
	systick_init();

	kprintf("=== Betriebssystem gestartet ===\n");
	test_kernel();

	scheduler_start();
}
