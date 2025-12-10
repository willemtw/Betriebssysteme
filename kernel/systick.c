#include "arch/cpu/interrupts.h"
#include "kernel/threading/scheduler.h"
#include <kernel/systick.h>
#include <arch/bsp/interrupt_controller.h>
#include <lib/kprintf.h>
#include <arch/bsp/system_timer.h>
#include <config.h>

void systick_init(void)
{
	interrupt_controller_enable_irq(SYSTICK_TIMER_IRQ_ID);
	system_timer_set_interval(SYSTICK_TIMER_CHANNEL, SYSTICK_TIMER_INTERVAL);
}

void systick_handle_irq(struct saved_registers *sp)
{
	if (!interrupt_controller_check_pending(SYSTICK_TIMER_IRQ_ID) ||
	    !system_timer_check_compare(SYSTICK_TIMER_CHANNEL)) {
		return;
	}
	system_timer_clear_interrupt(SYSTICK_TIMER_CHANNEL);
	system_timer_update_compare(SYSTICK_TIMER_CHANNEL, SYSTICK_TIMER_INTERVAL);

	kprintf("!\n");

	scheduler_tick_from_irq(sp);
}
