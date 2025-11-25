#ifndef EXCEPTION_PRINT_H
#define EXCEPTION_PRINT_H

#include <stdbool.h>

void print_exception_infos(const char *exception_name, bool is_data_abort,
			   bool is_prefetch_abort, void *sp);

extern bool irq_debug;

#endif // EXCEPTION_PRINT_H
