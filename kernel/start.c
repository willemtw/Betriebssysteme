#include <arch/bsp/yellow_led.h>
#include <arch/bsp/uart.h>
#include <lib/kprintf.h>
#include <config.h>

static volatile unsigned int counter = 0;

static void increment_counter(void)
{
	counter++;
}

void start_kernel [[noreturn]] (void);
void start_kernel [[noreturn]] (void)
{
	kprintf("=== Betriebssystem gestartet ===\n");
	test_kernel();
	//	init_yellow();
	//	yellow_on();
	//	yellow_off();
	//	while (true) {
	//		char c = uart_getc();
	//#pragma GCC diagnostic push
	//#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
	//		kprintf("Es wurde folgendes Zeichen eingegeben: %c, In Hexadezimal: %x, "
	//			"In Dezimal: %08i, Als unsigned: %u, Als Ptr: %p\n",
	//			c, (unsigned int)c, (int)c, (unsigned int)c, (void *)c);
	//#pragma GCC diagnostic pop
	//	}
	// Endless counter
	while (true) {
		increment_counter();
	}
}
