#include <stdint.h>

void do_svc(void)
{
	// If we're in SV mode already, lr will be overwritten by svc
	asm volatile("svc 1" ::: "lr", "memory");
}

void do_prefetch_abort(void)
{
	asm volatile("bkpt" ::: "memory");
}

void do_data_abort(void)
{
	uint8_t buffer[5];

	uint8_t *ptr = buffer;

	// If ptr is aligned, move it over one byte to force misalignment (for a 4-byte read)
	if ((uint32_t)ptr % 4 == 0) {
		ptr++;
	}

	// Paranoid casting so GCC doesn't somehow prevent this
	volatile uint32_t *misaligned = (uint32_t *)((uint32_t)(ptr));

	// Misaligned read to cause DA
	(*misaligned);
}

void do_undef(void)
{
	asm volatile("udf #0" ::: "memory");
}
