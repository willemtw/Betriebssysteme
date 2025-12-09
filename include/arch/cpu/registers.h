#ifndef REGISTERS_H
#define REGISTERS_H

#include <arch/cpu/modes.h>
#include <stdint.h>

struct psr {
	union {
		uint32_t r;

		struct {
			uint8_t	 mode : 5;
			uint8_t	 t : 1;
			uint8_t	 f : 1;
			uint8_t	 i : 1;
			uint8_t	 a : 1;
			uint8_t	 e : 1;
			uint32_t _irrelevant : 18;
			uint8_t	 v : 1;
			uint8_t	 c : 1;
			uint8_t	 z : 1;
			uint8_t	 n : 1;
		} d;
	};
};

static_assert(sizeof(struct psr) == 4);

struct psr read_cpsr(void);
struct psr read_spsr(void);
struct psr read_spsr_mode(enum cpu_mode mode);
void       write_spsr(struct psr spsr);
uint32_t   read_dfsr(void);
uint32_t   read_dfar(void);
uint32_t   read_ifsr(void);
uint32_t   read_ifar(void);
uint32_t   read_lr(void);
uint32_t   read_lr_mode(enum cpu_mode mode);
uint32_t   read_sp(void);
uint32_t   read_sp_mode(enum cpu_mode mode);
void       write_lr_mode(enum cpu_mode mode, uint32_t lr);
void       write_sp_mode(enum cpu_mode mode, uint32_t sp);

#endif // REGISTERS_H
