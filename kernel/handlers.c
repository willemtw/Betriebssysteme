#include <arch/cpu/interrupts.h>
#include <stdbool.h>
#include <lib/kprintf.h>
#include <arch/cpu/exception_print.h>
#include <kernel/systick.h>
#include <arch/bsp/uart.h>
#include <kernel/threads/scheduler.h>

void handle_irq(struct saved_registers *sp)
{
	uart_handle_irq();
	if (irq_debug) {
		print_exception_infos("Interrupt", false, false, sp);
	}
	systick_handle_irq(sp);
}

void handle_fiq(void *sp)
{
	if (irq_debug) {
		print_exception_infos("Fast Interrupt", false, false, sp);
	}
}

void handle_undefined_instruction(void *sp)
{
	print_exception_infos("Undefined Instruction", false, false, sp);
	scheduler_thread_terminate_running_from_irq(sp);
	//uart_putc('\4');
	//while (1)
	//	;
}

void handle_svc(void *sp)
{
	(void)sp;
	scheduler_thread_terminate_running_from_irq(sp);
	// print_exception_infos("Supervisor Call", false, false, sp);
	//uart_putc('\4');
	//while (1)
	//	;
}

void handle_prefetch_abort(void *sp)
{
	print_exception_infos("Prefetch Abort", false, true, sp);
	scheduler_thread_terminate_running_from_irq(sp);
	//uart_putc('\4');
	//while (1)
	//	;
}

void handle_data_abort(void *sp)
{
	print_exception_infos("Data Abort", true, false, sp);
	scheduler_thread_terminate_running_from_irq(sp);
	//uart_putc('\4');
	//while (1)
	//	;
}
