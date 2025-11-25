#include <arch/bsp/interrupt_controller.h>

void interrupt_controller_enable_irq(enum irq_id irq)
{
	if (irq <= 31) {
		INTERRUPT_CONTROLLER->enable_irqs_1 = (1 << irq);
	} else {
		INTERRUPT_CONTROLLER->enable_irqs_2 = (1 << (irq - 32));
	}
}

void interrupt_controller_disable_irq(enum irq_id irq)
{
	if (irq <= 31) {
		INTERRUPT_CONTROLLER->disable_irqs_1 = (1 << irq);
	} else {
		INTERRUPT_CONTROLLER->disable_irqs_2 = (1 << (irq - 32));
	}
}

bool interrupt_controller_check_pending(enum irq_id irq)
{
	if (irq <= 31) {
		return INTERRUPT_CONTROLLER->irq_pending_1 & (1 << irq);
	} else {
		return INTERRUPT_CONTROLLER->irq_pending_1 & (1 << (irq - 32));
	}
}
