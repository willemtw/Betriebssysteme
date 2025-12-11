#ifndef SYSTICK_H
#define SYSTICK_H

#include "arch/cpu/interrupts.h"
#include <config.h>

// C0/C2 are reserved for the GPU
#define SYSTICK_TIMER_CHANNEL  TIMER_C1
#define SYSTICK_TIMER_IRQ_ID   SYSTEM_TIMER_C1_IRQ
#define SYSTICK_TIMER_INTERVAL TIMER_INTERVAL

void systick_init(void);

void systick_handle_irq(struct saved_registers *sp);

void systick_postpone(void);

#endif // SYSTICK_H
