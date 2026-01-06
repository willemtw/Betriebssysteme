#include "arch/cpu/modes.h"
#include "kernel/syscalls.h"
#include <arch/cpu/interrupts.h>
#include <stdbool.h>
#include <lib/kprintf.h>
#include <arch/cpu/exception_print.h>
#include <kernel/systick.h>
#include <arch/bsp/uart.h>
#include <kernel/threading/scheduler.h>

void handle_irq(struct saved_registers *sp)
{
	scheduler_begin_exception(sp);
	uart_handle_irq();

	if (irq_debug) {
		print_exception_infos("Interrupt", false, false, sp);
	}

	systick_handle_irq();
}

void handle_fiq(struct saved_registers *sp)
{
	scheduler_begin_exception(sp);
	print_exception_infos("Fast Interrupt", false, false, sp);

	if (read_spsr().d.mode == CPU_MODE_USR) {
		scheduler_thread_terminate_running_from_irq();
	} else {
		uart_putc('\4');
		while (1)
			;
	}
}

void handle_undefined_instruction(struct saved_registers *sp)
{
	scheduler_begin_exception(sp);
	print_exception_infos("Undefined Instruction", false, false, sp);

	if (read_spsr().d.mode == CPU_MODE_USR) {
		scheduler_thread_terminate_running_from_irq();
	} else {
		uart_putc('\4');
		while (1)
			;
	}
}

void handle_svc(struct saved_registers *sp)
{
	scheduler_begin_exception(sp);

	if (read_spsr().d.mode == CPU_MODE_USR) {
		handle_syscall(sp);
	} else {
		print_exception_infos("Supervisor Call", false, false, sp);
		uart_putc('\4');
		while (1)
			;
	}
}

void handle_prefetch_abort(struct saved_registers *sp)
{
	scheduler_begin_exception(sp);
	print_exception_infos("Prefetch Abort", false, true, sp);

	if (read_spsr().d.mode == CPU_MODE_USR) {
		scheduler_thread_terminate_running_from_irq();
	} else {
		uart_putc('\4');
		while (1)
			;
	}
}

void handle_data_abort(struct saved_registers *sp)
{
	scheduler_begin_exception(sp);
	print_exception_infos("Data Abort", true, false, sp);

	if (read_spsr().d.mode == CPU_MODE_USR) {
		scheduler_thread_terminate_running_from_irq();
	} else {
		uart_putc('\4');
		while (1)
			;
	}
}
