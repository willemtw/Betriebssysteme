static void subprogram [[noreturn]] (void);

void start_kernel [[noreturn]] (void) {
	...
	kprintf("=== Betriebssystem gestartet ===\n");
	test_kernel();
	while(true) {
		char c = uart_getc();
		switch(c) {
			case 'd':
				irq_debug = !irq_debug;
				break;
			case 'a':
				do_data_abort();
				break;
			case 'p':
				do_prefetch_abort();
				break;
			case 's':
				do_supervisor_call();
				break;
			case 'u':
				do_undefined_inst();
				break;
			case 'c':
				register_checker();
				break;
			case 'e':
				subprogram();
			default:
				kprintf("Unknown input: %c\n", c);
				break;
		}
	}
}

static void subprogram [[noreturn]] (void) {
	while(true) {
		char c = uart_getc();
		for(unsigned int n = 0; n < PRINT_COUNT; n++) {
			uart_putc(c);
			volatile unsigned int i = 0;
			for(; i < BUSY_WAIT_COUNTER; i++) {}
		}
	}
}
