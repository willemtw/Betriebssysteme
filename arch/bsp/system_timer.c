#include <arch/bsp/system_timer.h>
#include <lib/kprintf.h>

void system_timer_set_interval(enum timer_channel channel, uint32_t interval_us)
{
	uint32_t current = SYSTEM_TIMER->clo;
	uint32_t compare = current + interval_us;

	switch (channel) {
	case TIMER_C0:
		SYSTEM_TIMER->c0 = compare;
		break;
	case TIMER_C1:
		SYSTEM_TIMER->c1 = compare;
		break;
	case TIMER_C2:
		SYSTEM_TIMER->c2 = compare;
		break;
	case TIMER_C3:
		SYSTEM_TIMER->c3 = compare;
		break;
	default:
		kprintf("Invalid timer channel: %i\n", channel);
		break;
	}
}

bool system_timer_check_compare(enum timer_channel channel)
{
	switch (channel) {
	case TIMER_C0:
		return SYSTEM_TIMER->cs.d.m0;
	case TIMER_C1:
		return SYSTEM_TIMER->cs.d.m1;
	case TIMER_C2:
		return SYSTEM_TIMER->cs.d.m2;
	case TIMER_C3:
		return SYSTEM_TIMER->cs.d.m3;
	default:
		kprintf("Invalid timer channel: %i\n", channel);
		return 0;
	}
}

void system_timer_clear_interrupt(enum timer_channel channel)
{
	struct system_timer_cs cs;
	cs.r = 0;

	switch (channel) {
	case TIMER_C0:
		cs.d.m0 = 1;
		break;
	case TIMER_C1:
		cs.d.m1 = 1;
		break;
	case TIMER_C2:
		cs.d.m2 = 1;
		break;
	case TIMER_C3:
		cs.d.m3 = 1;
		break;
	default:
		kprintf("Invalid timer channel: %i\n", channel);
		break;
	}

	SYSTEM_TIMER->cs.r = cs.r;
}

void system_timer_update_compare(enum timer_channel channel, uint32_t interval_us)
{
	switch (channel) {
	case TIMER_C0:
		SYSTEM_TIMER->c0 += interval_us;
		break;
	case TIMER_C1:
		SYSTEM_TIMER->c1 += interval_us;
		break;
	case TIMER_C2:
		SYSTEM_TIMER->c2 += interval_us;
		break;
	case TIMER_C3:
		SYSTEM_TIMER->c3 += interval_us;
		break;
	default:
		kprintf("Invalid timer channel: %i\n", channel);
		break;
	}
}

uint32_t system_timer_get_counter(void)
{
	return SYSTEM_TIMER->clo;
}
