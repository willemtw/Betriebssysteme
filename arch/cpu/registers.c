#include <arch/cpu/modes.h>
#include <arch/cpu/registers.h>
#include <stdbool.h>

static inline bool share_register_bank(enum cpu_mode mode1, enum cpu_mode mode2)
{
	if (mode1 == mode2) {
		return true;
	}
	return (mode1 == CPU_MODE_USR || mode1 == CPU_MODE_SYS) &&
	       (mode2 == CPU_MODE_USR || mode2 == CPU_MODE_SYS);
}

struct psr read_cpsr(void)
{
	struct psr cpsr;
	asm volatile("mrs %0, cpsr" : "=r"(cpsr.r));
	return cpsr;
}

struct psr read_spsr(void)
{
	struct psr spsr;
	asm volatile("mrs %0, spsr" : "=r"(spsr.r));
	return spsr;
}

uint32_t read_dfsr(void)
{
	uint32_t dfsr;
	asm volatile("mrc p15, 0, %0, c5, c0, 0" : "=r"(dfsr));
	return dfsr;
}

uint32_t read_dfar(void)
{
	uint32_t dfar;
	asm volatile("mrc p15, 0, %0, c6, c0, 0" : "=r"(dfar));
	return dfar;
}

uint32_t read_ifsr(void)
{
	uint32_t ifsr;
	asm volatile("mrc p15, 0, %0, c5, c0, 1" : "=r"(ifsr));
	return ifsr;
}

uint32_t read_ifar(void)
{
	uint32_t ifar;
	asm volatile("mrc p15, 0, %0, c6, c0, 2" : "=r"(ifar));
	return ifar;
}

uint32_t read_lr(void)
{
	uint32_t lr;
	asm volatile("mov %0, lr" : "=r"(lr));
	return lr;
}

uint32_t read_sp(void)
{
	uint32_t sp;
	asm volatile("mov %0, sp" : "=r"(sp));
	return sp;
}

uint32_t read_lr_mode(enum cpu_mode mode)
{
	uint32_t   lr;
	struct psr cpsr = read_cpsr();

	if (share_register_bank(cpsr.d.mode, mode)) {
		return read_lr();
	}

	switch (mode) {
	case CPU_MODE_USR:
	case CPU_MODE_SYS:
		asm volatile("mrs %0, lr_usr" : "=r"(lr));
		break;
	case CPU_MODE_IRQ:
		asm volatile("mrs %0, lr_irq" : "=r"(lr));
		break;
	case CPU_MODE_ABT:
		asm volatile("mrs %0, lr_abt" : "=r"(lr));
		break;
	case CPU_MODE_UND:
		asm volatile("mrs %0, lr_und" : "=r"(lr));
		break;
	case CPU_MODE_SVC:
		asm volatile("mrs %0, lr_svc" : "=r"(lr));
		break;
	case CPU_MODE_FIQ:
		asm volatile("mrs %0, lr_fiq" : "=r"(lr));
		break;
	default:
		lr = 0;
		break;
	}
	return lr;
}

uint32_t read_sp_mode(enum cpu_mode mode)
{
	uint32_t   sp;
	struct psr cpsr = read_cpsr();

	if (share_register_bank(cpsr.d.mode, mode)) {
		return read_sp();
	}

	switch (mode) {
	case CPU_MODE_USR:
	case CPU_MODE_SYS:
		asm volatile("mrs %0, sp_usr" : "=r"(sp));
		break;
	case CPU_MODE_IRQ:
		asm volatile("mrs %0, sp_irq" : "=r"(sp));
		break;
	case CPU_MODE_ABT:
		asm volatile("mrs %0, sp_abt" : "=r"(sp));
		break;
	case CPU_MODE_UND:
		asm volatile("mrs %0, sp_und" : "=r"(sp));
		break;
	case CPU_MODE_SVC:
		asm volatile("mrs %0, sp_svc" : "=r"(sp));
		break;
	case CPU_MODE_FIQ:
		asm volatile("mrs %0, sp_fiq" : "=r"(sp));
		break;
	default:
		sp = 0;
		break;
	}
	return sp;
}

struct psr read_spsr_mode(enum cpu_mode mode)
{
	struct psr spsr;
	struct psr cpsr = read_cpsr();

	if (cpsr.d.mode == mode) {
		return read_spsr();
	}

	spsr.r = 0;
	switch (mode) {
	case CPU_MODE_IRQ:
		asm volatile("mrs %0, spsr_irq" : "=r"(spsr.r));
		break;
	case CPU_MODE_ABT:
		asm volatile("mrs %0, spsr_abt" : "=r"(spsr.r));
		break;
	case CPU_MODE_UND:
		asm volatile("mrs %0, spsr_und" : "=r"(spsr.r));
		break;
	case CPU_MODE_SVC:
		asm volatile("mrs %0, spsr_svc" : "=r"(spsr.r));
		break;
	case CPU_MODE_FIQ:
		asm volatile("mrs %0, spsr_fiq" : "=r"(spsr.r));
		break;
	default:
		break;
	}
	return spsr;
}
