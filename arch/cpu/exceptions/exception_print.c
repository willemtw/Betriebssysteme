#include <lib/kprintf.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <arch/cpu/registers.h>
#include <arch/cpu/modes.h>
#include <arch/cpu/interrupts.h>
#include <arch/cpu/exception_print.h>

bool irq_debug = false;

struct mode_regs {
	uint32_t user_lr;
	uint32_t user_sp;
	uint32_t irq_lr;
	uint32_t irq_sp;
	uint32_t abort_lr;
	uint32_t abort_sp;
	uint32_t undefined_lr;
	uint32_t undefined_sp;
	uint32_t supervisor_lr;
	uint32_t supervisor_sp;
};

// Even though they are the same thing on this platform, we will get warnings if we don't
// cast uint32_t to "unsigned int" all over this file because uint32_t is defined as
// "long unsigned int" and the format specifier for that type is "%lx" which our kprintf
// doesn't support.
static_assert(sizeof(uint32_t) == sizeof(unsigned int));

static const char      *get_fsr_description(unsigned int fsr);
static void		print_psr(struct psr psr);
static struct mode_regs read_mode_specific_registers(void);

void print_exception_infos(const char *exception_name, bool is_data_abort, bool is_prefetch_abort,
			   void *sp)
{
	struct exception_stack_frame *frame = (struct exception_stack_frame *)sp;

	uint32_t exception_source_addr = frame->lr;
	uint32_t cpsr		       = read_cpsr().r;
	uint32_t svc_spsr	       = read_spsr_mode(CPU_MODE_SVC).r;
	uint32_t irq_spsr	       = read_spsr_mode(CPU_MODE_IRQ).r;
	uint32_t abt_spsr	       = read_spsr_mode(CPU_MODE_ABT).r;
	uint32_t und_spsr	       = read_spsr_mode(CPU_MODE_UND).r;

	kprintf("############ EXCEPTION ############\n");
	kprintf("%s an Adresse: 0x%08x\n", exception_name, (unsigned int)exception_source_addr);

	if (is_data_abort) {
		uint32_t    dfsr	     = read_dfsr();
		uint32_t    dfar	     = read_dfar();
		const char *dfsr_description = get_fsr_description(dfsr);
		kprintf("Data Fault Status Register: 0x%08x -> %s\n", (unsigned int)dfsr,
			dfsr_description);
		kprintf("Data Fault Adress Register: 0x%08x\n", (unsigned int)dfar);
	}

	if (is_prefetch_abort) {
		uint32_t    ifsr	     = read_ifsr();
		uint32_t    ifar	     = read_ifar();
		const char *ifsr_description = get_fsr_description(ifsr);
		kprintf("Instruction Fault Status Register: 0x%08x -> %s\n", (unsigned int)ifsr,
			ifsr_description);
		kprintf("Instruction Fault Adress Register: 0x%08x\n", (unsigned int)ifar);
	}

	kprintf("\n>> Registerschnappschuss <<\n");
	kprintf("R0: 0x%08x  R5: 0x%08x  R10: 0x%08x\n", (unsigned int)frame->r0,
		(unsigned int)frame->r5, (unsigned int)frame->r10);
	kprintf("R1: 0x%08x  R6: 0x%08x  R11: 0x%08x\n", (unsigned int)frame->r1,
		(unsigned int)frame->r6, (unsigned int)frame->r11);
	kprintf("R2: 0x%08x  R7: 0x%08x  R12: 0x%08x\n", (unsigned int)frame->r2,
		(unsigned int)frame->r7, (unsigned int)frame->r12);
	kprintf("R3: 0x%08x  R8: 0x%08x\n", (unsigned int)frame->r3, (unsigned int)frame->r8);
	kprintf("R4: 0x%08x  R9: 0x%08x\n", (unsigned int)frame->r4, (unsigned int)frame->r9);

	struct mode_regs mode_regs = read_mode_specific_registers();

	kprintf("\n>> Modusspezifische Register <<\n");
	kprintf("User/System | LR: 0x%08x | SP: 0x%08x | CPSR: ", (unsigned int)mode_regs.user_lr,
		(unsigned int)mode_regs.user_sp);
	print_psr((struct psr){ .r = cpsr });
	kprintf("\nIRQ         | LR: 0x%08x | SP: 0x%08x | SPSR: ", (unsigned int)mode_regs.irq_lr,
		(unsigned int)mode_regs.irq_sp);
	print_psr((struct psr){ .r = irq_spsr });
	kprintf("\nAbort       | LR: 0x%08x | SP: 0x%08x | SPSR: ",
		(unsigned int)mode_regs.abort_lr, (unsigned int)mode_regs.abort_sp);
	print_psr((struct psr){ .r = abt_spsr });
	kprintf("\nUndefined   | LR: 0x%08x | SP: 0x%08x | SPSR: ",
		(unsigned int)mode_regs.undefined_lr, (unsigned int)mode_regs.undefined_sp);
	print_psr((struct psr){ .r = und_spsr });
	kprintf("\nSupervisor  | LR: 0x%08x | SP: 0x%08x | SPSR: ",
		(unsigned int)mode_regs.supervisor_lr, (unsigned int)mode_regs.supervisor_sp);
	print_psr((struct psr){ .r = svc_spsr });
	kprintf("\n");
}

static void print_psr(struct psr psr)
{
	kprintf("%c%c%c%c %c %c%c%c", psr.d.n ? 'N' : '_', psr.d.z ? 'Z' : '_', psr.d.c ? 'C' : '_',
		psr.d.v ? 'V' : '_', psr.d.e ? 'E' : '_', psr.d.i ? 'I' : '_', psr.d.f ? 'F' : '_',
		psr.d.t ? 'T' : '_');

	const char *mode_name = get_mode_name(psr.d.mode);
	kprintf(" %8s", mode_name);
	kprintf(" 0x%08x", (unsigned int)psr.r);
}

static const char *get_fsr_description(unsigned int fsr)
{
	static const char *fsr_sources[] = {
		[0b00000] = "No function, reset value",
		[0b00001] = "Alignment fault",
		[0b00010] = "Debug event fault",
		[0b00011] = "Access Flag fault on Section",
		[0b00100] = "Cache maintenance operation fault",
		[0b00101] = "Translation fault on Section",
		[0b00110] = "Access Flag fault on Page",
		[0b00111] = "Translation fault on Page",
		[0b01000] = "Precise External Abort",
		[0b01001] = "Domain fault on Section",
		[0b01011] = "Domain fault on Page",
		[0b01100] = "External abort on Section",
		[0b01101] = "Permission fault on Section",
		[0b01110] = "External abort on Page",
		[0b01111] = "Permission fault on Page",
		[0b10000] = "TLB conflict abort",
		[0b10100] = "Implementation definedf fault",
		[0b10110] = "External Abort",
		[0b11000] = "Asynchronous parity error on memory access",
		[0b11001] = "Synchronous parity error on memory access",
		[0b11010] = "Implementation defined fault",
		[0b11100] = "Synchronous parity error on translation table walk on section",
		[0b11110] = "Synchronous parity error on translation table walk on page",
	};

	const int    fsr_status_4_index = 10;
	unsigned int fsr_status = (fsr & 0b1111) | ((fsr & (1 << (fsr_status_4_index - 1))) >>
						    (fsr_status_4_index - 4 - 1));

	if (fsr_status > sizeof(fsr_sources) / sizeof(const char *) ||
	    fsr_sources[fsr_status] == NULL) {
		return "Invalid fault status register value";
	}

	return fsr_sources[fsr_status];
}

static struct mode_regs read_mode_specific_registers(void)
{
	struct mode_regs regs;

	regs.user_lr	   = read_lr_mode(CPU_MODE_USR);
	regs.user_sp	   = read_sp_mode(CPU_MODE_USR);
	regs.irq_lr	   = read_lr_mode(CPU_MODE_IRQ);
	regs.irq_sp	   = read_sp_mode(CPU_MODE_IRQ);
	regs.abort_lr	   = read_lr_mode(CPU_MODE_ABT);
	regs.abort_sp	   = read_sp_mode(CPU_MODE_ABT);
	regs.undefined_lr  = read_lr_mode(CPU_MODE_UND);
	regs.undefined_sp  = read_sp_mode(CPU_MODE_UND);
	regs.supervisor_lr = read_lr_mode(CPU_MODE_SVC);
	regs.supervisor_sp = read_sp_mode(CPU_MODE_SVC);

	return regs;
}
