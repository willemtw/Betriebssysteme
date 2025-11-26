#ifndef SYSTEM_TIMER_H
#define SYSTEM_TIMER_H

#include <stdint.h>
#include <stdbool.h>

#define SYSTEM_TIMER_BASE 0x3F003000

struct system_timer_cs {
	union {
		uint32_t r;

		struct {
			uint8_t	 m0 : 1;
			uint8_t	 m1 : 1;
			uint8_t	 m2 : 1;
			uint8_t	 m3 : 1;
			uint32_t _reserved : 28;
		} d;
	};
};

struct system_timer_regs {
	struct system_timer_cs cs;
	uint32_t	       clo;
	uint32_t	       chi;
	uint32_t	       c0;
	uint32_t	       c1;
	uint32_t	       c2;
	uint32_t	       c3;
};

enum timer_channel {
	TIMER_C0 = 0,
	TIMER_C1 = 1,
	TIMER_C2 = 2,
	TIMER_C3 = 3,
};

#define SYSTEM_TIMER ((volatile struct system_timer_regs *)SYSTEM_TIMER_BASE)

void	 system_timer_set_interval(enum timer_channel channel, uint32_t interval_us);
void	 system_timer_clear_interrupt(enum timer_channel channel);
bool	 system_timer_check_compare(enum timer_channel channel);
void	 system_timer_update_compare(enum timer_channel channel, uint32_t interval_us);
uint32_t system_timer_get_counter(void);

#endif // SYSTEM_TIMER_H
