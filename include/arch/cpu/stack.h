#include <stdint.h>

extern uint8_t fiq_stack[2048];
extern uint8_t irq_stack[2048];
extern uint8_t abt_stack[2048];
extern uint8_t und_stack[2048];
extern uint8_t sys_stack[4096];
extern uint8_t svc_stack[2048];
