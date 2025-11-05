#ifndef KPRINTF_H
#define KPRINTF_H

void kprintf [[gnu::format(printf, 1, 2)]] (const char *format, ...);

void kprintf(const char *format, ...);

#endif
