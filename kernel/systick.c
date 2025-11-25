#include "arch/bsp/interrupt_controller.h"
#include <lib/kprintf.h>
#include <arch/bsp/system_timer.h>
#include <config.h>

void systick_init(void)
{
	interrupt_controller_enable_irq(SYSTEM_TIMER_C0_IRQ);
	system_timer_set_interval(TIMER_C0, TIMER_INTERVAL);
}

void systick_handle_irq(void)
{
	if (!interrupt_controller_check_pending(SYSTEM_TIMER_C0_IRQ) ||
	    !system_timer_check_compare(TIMER_C0)) {
		return;
	}
	system_timer_clear_interrupt(TIMER_C0);
	system_timer_update_compare(TIMER_C0, TIMER_INTERVAL);

	kprintf("!\n");
}
