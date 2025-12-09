#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stdint.h>

struct saved_registers {
	uint32_t r0;
	uint32_t r1;
	uint32_t r2;
	uint32_t r3;
	uint32_t r4;
	uint32_t r5;
	uint32_t r6;
	uint32_t r7;
	uint32_t r8;
	uint32_t r9;
	uint32_t r10;
	uint32_t r11;
	uint32_t r12;
	uint32_t pc;
};

void setup_interrupts(void);
void enable_irq(void);
void disable_irq(void);

#endif // INTERRUPTS_H
