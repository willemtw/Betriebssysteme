#include <stdint.h>

__attribute__((aligned(8), section(".stack.fiq"))) uint8_t fiq_stack[2048];
__attribute__((aligned(8), section(".stack.irq"))) uint8_t irq_stack[2048];
__attribute__((aligned(8), section(".stack.abt"))) uint8_t abt_stack[2048];
__attribute__((aligned(8), section(".stack.und"))) uint8_t und_stack[2048];
__attribute__((aligned(8), section(".stack.sys"))) uint8_t sys_stack[4096];
__attribute__((aligned(8), section(".stack.svc"))) uint8_t svc_stack[2048];
