#ifndef INTERRUPT_CONTROLLER_H
#define INTERRUPT_CONTROLLER_H

#include <stdint.h>

#define INTERRUPT_CONTROLLER_BASE 0x3F00B200

enum irq_id {
	SYSTEM_TIMER_C0_IRQ = 0,
	SYSTEM_TIMER_C1_IRQ = 1,
	SYSTEM_TIMER_C2_IRQ = 2,
	SYSTEM_TIMER_C3_IRQ = 3,
	UART_IRQ	    = 57,
};

struct interrupt_controller_regs {
	uint32_t irq_basic_pending;
	uint32_t irq_pending_1;
	uint32_t irq_pending_2;
	uint32_t fiq_control;
	uint32_t enable_irqs_1;
	uint32_t enable_irqs_2;
	uint32_t enable_basic_irqs;
	uint32_t disable_irqs_1;
	uint32_t disable_irqs_2;
	uint32_t disable_basic_irqs;
};

#define INTERRUPT_CONTROLLER \
	((volatile struct interrupt_controller_regs *)INTERRUPT_CONTROLLER_BASE)

void interrupt_controller_enable_irq(enum irq_id irq);
void interrupt_controller_disable_irq(enum irq_id irq);
bool interrupt_controller_check_pending(enum irq_id id);

#endif // INTERRUPT_CONTROLLER_H
