#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <kernel/threading/thread.h>

#define NUM_THREADS 32

#ifndef __ASSEMBLER__
#include <arch/cpu/interrupts.h>
#include <stddef.h>
#include <kernel/result.h>
#include <lib/list.h>

void scheduler_begin_exception(struct saved_registers *regs);

void scheduler_prepare_svc_return(uint32_t value);

void scheduler_sleep_running_from_irq(uint32_t cycles);

void scheduler_wait_running_from_irq(list_node *wait_queue);

enum result scheduler_notify_one_from_irq(list_node *wait_queue, uint32_t value);

void scheduler_thread_create(void (*func)(void *), const void *arg, size_t arg_size);

void scheduler_thread_terminate_running_from_irq();

void scheduler_tick_from_irq();

[[noreturn]] void scheduler_start(void);

#endif // __ASSEMBLER__

#endif // SCHEDULER_H
