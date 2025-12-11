#include <kernel/threading/scheduler.h>
#include <lib/kprintf.h>
#include <arch/bsp/uart.h>
#include <arch/cpu/exception_print.h>
#include <tests/regcheck.h>
#include <config.h>
#include <kernel/systick.h>
#include <arch/cpu/interrupts.h>
#include <arch/cpu/registers.h>
#include <tests/exceptions.h>
#include <user/main.h>

//struct test_arg {
//	uint32_t    a;
//	uint8_t	    b;
//	uint16_t    c;
//	const char *name;
//	uint32_t    d;
//};
//
//void arg_test_thread(void *arg_void)
//{
//	struct test_arg *arg = (struct test_arg *)arg_void;
//	kprintf("a: %u,\nb: %u,\nc: %u,\nname: %s,\nd: %u", arg->a, arg->b, arg->c, arg->name,
//		arg->d);
//}

void start_kernel [[noreturn]] (void)
{
	uart_init();
	systick_init();
	setup_interrupts();

	kprintf("=== Betriebssystem gestartet ===\n");
	test_kernel();

	//	struct test_arg arg = {
	//		.a    = 27,
	//		.b    = 10,
	//		.c    = 0xff,
	//		.name = "This is a name",
	//		.d    = 0b101010101,
	//	};
	//
	//	scheduler_thread_create(arg_test_thread, &arg, sizeof(arg));
	//
	scheduler_start();
}
