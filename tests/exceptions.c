#include <stdint.h>

void do_supervisor_call(void)
{
	// If we're in SV mode already, lr will be overwritten by svc
	asm volatile("svc 1" ::: "lr");
}

void do_prefetch_abort(void)
{
	asm volatile("bkpt");
}

void do_data_abort(void)
{
	uint8_t buffer[8];

	for (uint8_t *ptr = buffer; ptr < buffer + 8; ptr++) {
		if ((uint32_t)ptr % 4) {
			volatile uint32_t *unaligned = (uint32_t *)((uint32_t)ptr);
			*(unaligned);
			return;
		}
	}
}

void do_undefined_inst(void)
{
	asm volatile("udf #0");
}
