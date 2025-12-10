#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <kernel/threading/thread.h>

#define NUM_THREADS 32

#ifndef __ASSEMBLER__
#include <arch/cpu/interrupts.h>
#include <stddef.h>

void scheduler_thread_create(void (*func)(void *), const void *arg, size_t arg_size);

void scheduler_thread_terminate_running_from_irq(struct saved_registers *sp);

void scheduler_tick_from_irq(struct saved_registers *sp);

[[noreturn]] void scheduler_start(void);

#endif // __ASSEMBLER__

#endif // SCHEDULER_H
