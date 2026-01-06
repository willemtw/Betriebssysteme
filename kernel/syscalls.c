#include <arch/cpu/exception_print.h>
#include <arch/cpu/interrupts.h>
#include <kernel/threading/scheduler.h>
#include <arch/bsp/uart.h>
#include <kernel/syscalls.h>
#include <lib/kprintf.h>

syscall_handler svc_handlers[NUM_SYSCALLS] = {
	sys_exit, sys_create_thread, sys_getc, sys_putc, sys_sleep, sys_kernel_exit,
};

void sys_exit(struct saved_registers *regs)
{
	(void)regs;
	scheduler_thread_terminate_running_from_irq();
}

void sys_create_thread(struct saved_registers *regs)
{
	scheduler_thread_create((void (*)(void *))regs->r1, (void *)regs->r2, regs->r3);
}

void sys_sleep(struct saved_registers *regs)
{
	scheduler_sleep_running_from_irq(regs->r1);
}

void sys_getc(struct saved_registers *regs)
{
	(void)regs;
	uart_user_getc();
}

void sys_putc(struct saved_registers *regs)
{
	uart_putc(regs->r1);
}

void sys_kernel_exit(struct saved_registers *regs)
{
	(void)regs;
	register uint32_t r0 asm("r0") = 6;
	asm volatile("svc #0" : "+r"(r0) : : "r1", "r2", "r3", "r4", "lr", "memory");
}

void handle_syscall(struct saved_registers *regs)
{
	uint32_t svc_num = regs->r0;
	if (svc_num >= NUM_SYSCALLS) {
		print_exception_infos("Unknown Supervisor Call", false, false, regs);
		scheduler_thread_terminate_running_from_irq();
		return;
	}

	syscall_handler handler = svc_handlers[svc_num];

	handler(regs);
}
