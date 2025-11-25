#include <stdint.h>
void do_supervisor_call(void)
{
	asm volatile("svc 1");
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
