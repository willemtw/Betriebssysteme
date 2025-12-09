#include <arch/cpu/modes.h>
#include <arch/cpu/registers.h>
#include <stdbool.h>

static inline bool share_register_bank(enum cpu_mode mode1, enum cpu_mode mode2)
{
	if (mode1 == mode2) {
		return true;
	}

	// SYS and USR share the same registers
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

void write_spsr(struct psr spsr)
{
	asm volatile("msr spsr, %0" : : "r"(spsr.r));
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
	// Access via VE is UB if normal access is possible
	if (share_register_bank(read_cpsr().d.mode, mode)) {
		return read_lr();
	}

	uint32_t lr;

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
	// Access via VE is UB if normal access is possible
	if (share_register_bank(read_cpsr().d.mode, mode)) {
		return read_sp();
	}

	uint32_t sp;

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
	struct psr cpsr = read_cpsr();

	struct psr spsr = { 0 };

	// USR doesn't have it's own SPSR and can't read any other SPSR
	if (cpsr.d.mode == CPU_MODE_USR) {
		return spsr;
	}

	if (read_cpsr().d.mode == mode) {
		return read_spsr();
	}

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

void write_lr_mode(enum cpu_mode mode, uint32_t lr)
{
	// Access via VE is UB if normal access is possible
	if (share_register_bank(read_cpsr().d.mode, mode)) {
		asm volatile("mov lr, %0" : : "r"(lr));
		return;
	}

	switch (mode) {
	case CPU_MODE_USR:
	case CPU_MODE_SYS:
		asm volatile("msr lr_usr, %0" : : "r"(lr));
		break;
	case CPU_MODE_IRQ:
		asm volatile("msr lr_irq, %0" : : "r"(lr));
		break;
	case CPU_MODE_ABT:
		asm volatile("msr lr_abt, %0" : : "r"(lr));
		break;
	case CPU_MODE_UND:
		asm volatile("msr lr_und, %0" : : "r"(lr));
		break;
	case CPU_MODE_SVC:
		asm volatile("msr lr_svc, %0" : : "r"(lr));
		break;
	case CPU_MODE_FIQ:
		asm volatile("msr lr_fiq, %0" : : "r"(lr));
		break;
	default:
		break;
	}
}

void write_sp_mode(enum cpu_mode mode, uint32_t sp)
{
	// Access via VE is UB if normal access is possible
	if (share_register_bank(read_cpsr().d.mode, mode)) {
		asm volatile("mov sp, %0" : : "r"(sp));
		return;
	}

	switch (mode) {
	case CPU_MODE_USR:
	case CPU_MODE_SYS:
		asm volatile("msr sp_usr, %0" : : "r"(sp));
		break;
	case CPU_MODE_IRQ:
		asm volatile("msr sp_irq, %0" : : "r"(sp));
		break;
	case CPU_MODE_ABT:
		asm volatile("msr sp_abt, %0" : : "r"(sp));
		break;
	case CPU_MODE_UND:
		asm volatile("msr sp_und, %0" : : "r"(sp));
		break;
	case CPU_MODE_SVC:
		asm volatile("msr sp_svc, %0" : : "r"(sp));
		break;
	case CPU_MODE_FIQ:
		asm volatile("msr sp_fiq, %0" : : "r"(sp));
		break;
	default:
		break;
	}
}
