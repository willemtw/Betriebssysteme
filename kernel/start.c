#include <arch/bsp/yellow_led.h>
#include <arch/bsp/uart.h>
#include <lib/kprintf.h>
#include <config.h>
#include <limits.h>

void start_kernel [[noreturn]] (void);
void start_kernel [[noreturn]] (void)
{
	kprintf("=== Betriebssystem gestartet ===\n");
	test_kernel();
	while (true) {
		char c = uart_getc();
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
		kprintf("Es wurde folgendes Zeichen eingegeben: %c, In Hexadezimal: %x, "
			"In Dezimal: %08i, Als Ptr: %p\n",
			c, (unsigned int)c, (int)c, (void *)c);
#pragma GCC diagnostic pop
	}
}
