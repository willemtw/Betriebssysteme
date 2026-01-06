#ifndef KSYSCALLS_H
#define KSYSCALLS_H
#include <arch/cpu/interrupts.h>

#define NUM_SYSCALLS 5

typedef void (*syscall_handler)(struct saved_registers *);

void sys_exit(struct saved_registers *regs);
void sys_create_thread(struct saved_registers *regs);
void sys_sleep(struct saved_registers *regs);
void sys_getc(struct saved_registers *regs);
void sys_putc(struct saved_registers *regs);

void handle_syscall(struct saved_registers *regs);
#endif // KSYSCALLS_H
