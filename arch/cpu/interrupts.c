#include <stdint.h>
#include <lib/kprintf.h>

extern uint32_t _ivt;

static inline void set_vbar(uint32_t addr)
{
	asm volatile("mcr p15, 0, %0, c12, c0, 0" : : "r"(addr));
}

static inline void enable_irq(void)
{
	asm volatile("cpsie i");
}

static inline void disable_irq(void)
{
	asm volatile("cpsie i");
}

void setup_interrupts(void)
{
	set_vbar((uint32_t)&_ivt);
	enable_irq();
}

void handle_irq(void *sp)
{
	(void)sp;
	kprintf("irq\n");
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
	(void)sp;
	kprintf("svc\n");
	//	while (1) {
	//	}
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
