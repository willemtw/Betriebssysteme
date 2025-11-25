#include <stdbool.h>
#include <lib/kprintf.h>
#include <arch/cpu/exception_print.h>
#include <arch/cpu/interrupts.h>
#include <kernel/systick.h>

void handle_irq(void *sp)
{
	systick_handle_irq();
	kprintf("!\n");
	if (irq_debug) {
		print_exception_infos("Interrupt", false, false, sp);
	}
}

void handle_fiq(void *sp)
{
	if (irq_debug) {
		print_exception_infos("Fast Interrupt", false, false, sp);
	}
}

void handle_undefined_instruction(void *sp)
{
	if (irq_debug) {
		print_exception_infos("Undefined Instruction", false, false, sp);
	}
}

void handle_svc(void *sp)
{
	if (irq_debug) {
		print_exception_infos("Supervisor Call", false, false, sp);
	}
}

void handle_prefetch_abort(void *sp)
{
	if (irq_debug) {
		print_exception_infos("Prefetch Abort", false, true, sp);
	}
}

void handle_data_abort(void *sp)
{
	if (irq_debug) {
		print_exception_infos("Data Abort", true, false, sp);
	}
}
