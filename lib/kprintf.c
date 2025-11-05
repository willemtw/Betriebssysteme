#include <lib/kprintf.h>
#include <arch/bsp/uart.h>
#include <stdarg.h>

static void print_in_base(unsigned int value, int base, int width, char pad)
{
	char buffer[16];
	int  i = 0;
	do {
		unsigned int digit = value % base;
		buffer[i++]	   = (digit < 10) ? '0' + digit : 'A' + digit - 10;
		value /= base;
	} while (value > 0);

	while (i < width) {
		buffer[i++] = pad;
	}
	while (i > 0) {
		uart_putc(buffer[--i]);
	}
}

uint8_t check_for_unknown_format(const char *format)
{
	while (*format) {
		while (*format != 0 && *format != '%') {
			format++;
		}
		if (*format == 0) {
			return 0;
		}
		format++;
		if (*format == 0) {
			return 0;
		}
		if (*format == '0') {
			format++;
			if (format == 0) {
				return 0;
			}
		}
		if (*format >= '0' && *format <= '9') {
			format++;
			if (format == 0) {
				return 0;
			}
		}
		switch (*format) {
		case 'c':
		case 's':
		case 'i':
		case 'u':
		case 'x':
		case 'p':
		case '%': {
			format++;
			continue;
		}
		default:
			return 1;
		}
	}
	return 0;
}

void kprintf(const char *format, ...)
{
	va_list args;
	va_start(args, format);

	if (check_for_unknown_format(format)) {
		kprintf("Unknown conversion specifier\n");
		return;
	}
	while (*format) {
		if (*format != '%') {
			uart_putc(*format++);
			continue;
		}
		format++;
		if (*format == 0) {
			return;
		}

		char pad   = ' ';
		int  width = 0;
		if (*format == '0') {
			pad = '0';
			format++;
			if (*format == 0) {
				return;
			}
		}
		if (*format >= '0' && *format <= '9') {
			width = *format - '0';
			format++;
			if (*format == 0) {
				return;
			}
		}

		switch (*format) {
		case 'c': {
			char c = va_arg(args, int);
			uart_putc(c);
			break;
		}
		case 's': {
			char *str = va_arg(args, char *);
			while (*str) {
				uart_putc(*str++);
			}
			break;
		}
		case 'i': {
			int val = va_arg(args, int);
			if (val < 0) {
				uart_putc('-');
				val = -val;
			}
			print_in_base(val, 10, width, pad);
			break;
		}
		case 'u': {
			unsigned int val = va_arg(args, unsigned int);
			print_in_base(val, 10, width, pad);
			break;
		}
		case 'x': {
			unsigned int val = va_arg(args, unsigned int);
			print_in_base(val, 16, width, pad);
			break;
		}
		case 'p': {
			uart_putc('0');
			uart_putc('x');
			width		 = 8;
			pad		 = '0';
			unsigned int val = va_arg(args, unsigned int);
			print_in_base(val, 16, width, pad);
			break;
		}
		case '%': {
			uart_putc('%');
			break;
		}
		default: {
			kprintf("Unknown conversion specifier\n");
		}
		}
		format++;
	}
}
