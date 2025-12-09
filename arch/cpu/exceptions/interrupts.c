#include <stdint.h>

extern uint32_t _ivt;

static inline void set_vbar(uint32_t addr)
{
	asm volatile("mcr p15, 0, %0, c12, c0, 0" : : "r"(addr));
}

void enable_irq(void)
{
	asm volatile("cpsie i");
}

void disable_irq(void)
{
	asm volatile("cpsid i");
}

void setup_interrupts(void)
{
	set_vbar((uint32_t)&_ivt);
	enable_irq();
}
