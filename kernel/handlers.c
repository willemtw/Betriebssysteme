#include "arch/cpu/modes.h"
#include <arch/cpu/interrupts.h>
#include <stdbool.h>
#include <lib/kprintf.h>
#include <arch/cpu/exception_print.h>
#include <kernel/systick.h>
#include <arch/bsp/uart.h>
#include <kernel/threads/scheduler.h>

void handle_irq(struct saved_registers *sp)
{
	disable_irq();
	uart_handle_irq();

	if (irq_debug) {
		print_exception_infos("Interrupt", false, false, sp);
	}
	systick_handle_irq(sp);
	enable_irq();
}

void handle_fiq(void *sp)
{
	disable_irq();
	if (irq_debug) {
		print_exception_infos("Fast Interrupt", false, false, sp);
	}
	if (read_spsr().d.mode == CPU_MODE_USR) {
		scheduler_thread_terminate_running_from_irq(sp);
	} else {
		uart_putc('\4');
		while (1)
			;
	}
	enable_irq();
}

void handle_undefined_instruction(void *sp)
{
	disable_irq();
	print_exception_infos("Undefined Instruction", false, false, sp);

	if (read_spsr().d.mode == CPU_MODE_USR) {
		scheduler_thread_terminate_running_from_irq(sp);
	} else {
		uart_putc('\4');
		while (1)
			;
	}
	enable_irq();
}

void handle_svc(void *sp)
{
	disable_irq();
	(void)sp;

	if (read_spsr().d.mode == CPU_MODE_USR) {
		scheduler_thread_terminate_running_from_irq(sp);
	}
	enable_irq();
}

void handle_prefetch_abort(void *sp)
{
	disable_irq();
	print_exception_infos("Prefetch Abort", false, true, sp);

	if (read_spsr().d.mode == CPU_MODE_USR) {
		scheduler_thread_terminate_running_from_irq(sp);
	} else {
		uart_putc('\4');
		while (1)
			;
	}
	enable_irq();
}

void handle_data_abort(void *sp)
{
	disable_irq();
	print_exception_infos("Data Abort", true, false, sp);

	if (read_spsr().d.mode == CPU_MODE_USR) {
		scheduler_thread_terminate_running_from_irq(sp);
	} else {
		uart_putc('\4');
		while (1)
			;
	}
	enable_irq();
}
