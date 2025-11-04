#include <lib/kprintf.h>
#include "uart.h"
#include <stdarg.h>

static void convert(unsigned int value, int base, int width, char pad){
  char buffer[16];
  int i = 0;
  do {
    unsigned int digit = value % base;
    buffer[i++] = (digit < 10) ? '0' + digit : 'A' + digit - 10;
    value /= base;
  } while (value > 0);

  while (i < width) {
    buffer[i++] = pad;
  }
  while (i > 0) {
    uart_putc(buffer[--i]);
  }
}

void kprintf(const char *format, ...){
  va_list args;
  va_start(args, format);

  while (*format){
    if(*format != '%'){
      uart_putc(*format++);
      continue;
    }
    format++;

    char pad = ' ';
    int width = 0;
    if(*format == '0'){
      pad = '0';
      format++;
    }
    if(*format >= '0' && *format <= '9'){
      width = *format - '0';
      format++;
    }

    switch(*format){
      case 'c':{
        char c = va_arg(args, int);
        uart_putc(c);
        break;
      }
      case 's':{
        char *str = va_arg(args, char *);
        while(*str){
          uart_putc(*str++);
        }
        break;
      }
      case 'i':{
        int val = va_arg(args, int);
        if(val < 0){
          uart_putc('-');
          val = -val;
        }
        convert(val, 10, width, pad);
        break;
      }
      case 'u':{
        unsigned int val = va_arg(args, unsigned int);
        convert(val, 10, width, pad);
        break;
      }
      case 'x':{
        unsigned int val = va_arg(args, unsigned int);
        convert(val, 16, width, pad);
        break;
      }
      case 'p':{
        uart_putc('0');
        uart_putc('x');
        unsigned int val = va_arg(args, unsigned int);
        convert(val, 16, width, pad);
        break;
      }
      case '%':{
        uart_putc('%');
        break;
      }
      default:{
        
      }
    }
  }
}