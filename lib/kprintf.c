#include <lib/kprintf.h>
#include <arch/bsp/uart.h>
#include <stdarg.h>
#include <stdint.h>

static int convert_to_base_string(unsigned int value, int base, char *buffer)
{
	int i = 0;
	do {
		unsigned int digit = value % base;
		buffer[i++]	   = (digit < 10) ? '0' + digit : 'a' + digit - 10;
		value /= base;
	} while (value > 0);
	return i;
}

static void print_in_base_signed(int value, int base, int width, char pad)
{
	char	     buffer[16];
	int	     is_negative = value < 0;
	unsigned int abs	 = is_negative ? (0u - (unsigned int)value) : (unsigned int)value;
	int	     i		 = convert_to_base_string(abs, base, buffer);

	int padding_needed = i >= width ? 0 : (width - i - is_negative);

	if (is_negative && pad == ' ') {
		buffer[i++] = '-';
	}

	for (int j = 0; j < padding_needed; j++) {
		buffer[i++] = pad;
	}

	if (is_negative && pad == '0') {
		buffer[i++] = '-';
	}

	while (i > 0) {
		uart_putc(buffer[--i]);
	}
}

static void print_in_base_unsigned(unsigned int value, int base, int width, char pad)
{
	char buffer[16];

	int i = convert_to_base_string(value, base, buffer);

	int padding_needed = i >= width ? 0 : (width - i);

	for (int j = 0; j < padding_needed; j++) {
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
		while (*format >= '0' && *format <= '9') {
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

void parse_width(const char **format, uint32_t *width)
{
	char	 buff[2]    = { '0', '0' };
	uint32_t num_digits = 0;

	while (**format != 0 && **format <= '9' && **format >= '0' && num_digits < 2) {
		buff[num_digits] = **format;
		num_digits++;
		(*format)++;
	}

	if (num_digits == 0) {
		return;
	}

	*width = num_digits == 1 ? buff[0] - '0' : (buff[0] - '0') * 10 + buff[1] - '0';
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

		char	 pad   = ' ';
		uint32_t width = 0;

		if (*format == '0') {
			pad = '0';
			format++;
			if (*format == 0) {
				return;
			}
		}

		parse_width(&format, &width);

		if (*format == 0) {
			return;
		}

		switch (*format) {
		case 'c': {
			char c = va_arg(args, int);
			uart_putc(c);
			break;
		}
		case 's': {
			char	*str	 = va_arg(args, char *);
			uint32_t str_len = 0;
			char	*temp	 = str;
			while (*temp++) {
				str_len++;
			}

			uint32_t padding_needed = str_len >= width ? 0 : (width - str_len);
			for (uint32_t j = 0; j < padding_needed; j++) {
				uart_putc(pad);
			}

			while (*str) {
				uart_putc(*str++);
			}
			break;
		}
		case 'i': {
			int val = va_arg(args, int);
			print_in_base_signed(val, 10, width, pad);
			break;
		}
		case 'u': {
			unsigned int val = va_arg(args, unsigned int);
			print_in_base_unsigned(val, 10, width, pad);
			break;
		}
		case 'x': {
			unsigned int val = va_arg(args, unsigned int);
			print_in_base_unsigned(val, 16, width, pad);
			break;
		}
		case 'p': {
			uart_putc('0');
			uart_putc('x');
			width		 = 8;
			pad		 = '0';
			unsigned int val = va_arg(args, unsigned int);
			print_in_base_unsigned(val, 16, width, pad);
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
