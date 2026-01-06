#include <kernel/threading/scheduler.h>
#include <lib/kprintf.h>
#include <arch/bsp/uart.h>
#include <arch/cpu/exception_print.h>
#include <tests/regcheck.h>
#include <config.h>
#include <kernel/systick.h>
#include <arch/cpu/interrupts.h>
#include <arch/cpu/registers.h>
#include <tests/exceptions.h>

[[gnu::weak]] void main(void)
{
	kprintf("Default main - user main not found\n");
}

void start_kernel [[noreturn]] (void)
{
	uart_init();
	systick_init();
	setup_interrupts();

	kprintf("=== Betriebssystem gestartet ===\n");
	test_kernel();

	scheduler_thread_create((void (*)(void *))main, NULL, 0);
	scheduler_start();
}
