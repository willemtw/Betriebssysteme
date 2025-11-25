#include <stdint.h>
#include <stdbool.h>
#include <lib/kprintf.h>
#include <arch/cpu/exception_print.h>
#include <arch/cpu/interrupts.h>
#include <kernel/systick.h>

void handle_irq(void *sp)
{
	(void)sp;
	// kprintf("irq\n");
	systick_handle_irq();
}

void handle_fiq(void *sp)
{
	(void)sp;
	kprintf("fiq\n");
}

void handle_undefined_instruction(void *sp)
{
	(void)sp;
	kprintf("undefined instruction\n");
}

void handle_svc(void *sp)
{
	print_exception_infos("SVC (Supervisor Call)", false, false, sp);
	kprintf("\nReturning from SVC handler...\n");
}

void handle_prefetch_abort(void *sp)
{
	(void)sp;
	kprintf("prefetch abort\n");
	while (1) {
	}
}

void handle_data_abort(void *sp)
{
	(void)sp;
	kprintf("data abort\n");
}
