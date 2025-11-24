#ifndef INTERRUPTS_H
#define INTERRUPTS_H

void setup_interrupts(void);

void handle_irq(void *sp);

void handle_fiq(void *sp);

void handle_undefined_instruction(void *sp);

void handle_svc(void *sp);

void handle_prefetch_abort(void *sp);

void handle_data_abort(void *sp);

#endif
