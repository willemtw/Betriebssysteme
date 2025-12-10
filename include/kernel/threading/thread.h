#ifndef THREAD_H
#define THREAD_H

#define THREAD_STACK_SIZE 4096

#define THREAD_CONTEXT_REGS 0
#define THREAD_CONTEXT_PC   52
#define THREAD_CONTEXT_LR   56
#define THREAD_CONTEXT_SP   60
#define THREAD_CONTEXT_CPSR 64

#ifndef __ASSEMBLER__
#include <arch/cpu/registers.h>
#include <stddef.h>
#include <stdint.h>

enum thread_status {
	THREAD_STATUS_TERMINATED = 0,
	THREAD_STATUS_READY,
	THREAD_STATUS_RUNNING,
	THREAD_STATUS_WAITING,
};

struct thread_context {
	uint32_t   r0;
	uint32_t   r1;
	uint32_t   r2;
	uint32_t   r3;
	uint32_t   r4;
	uint32_t   r5;
	uint32_t   r6;
	uint32_t   r7;
	uint32_t   r8;
	uint32_t   r9;
	uint32_t   r10;
	uint32_t   r11;
	uint32_t   r12;
	uint32_t   pc;
	uint32_t   lr;
	uint32_t   sp;
	struct psr cpsr;
};

struct thread {
	uint32_t id;
	void (*fn)(void *arg);
	enum thread_status    status;
	struct thread_context context;
	_Alignas(8) uint8_t stack[THREAD_STACK_SIZE];
};

void thread_init(struct thread *thread, void (*fn)(void *), const void *arg, size_t arg_size);

[[noreturn]] void thread_run(struct thread *thread);

void (*get_current_thread_fn(void))(void *);

struct thread_context *get_current_thread_context(void);

// Verify offsets match struct layout
static_assert(offsetof(struct thread_context, r0) == THREAD_CONTEXT_REGS,
	      "THREAD_CONTEXT_REGS mismatch");
static_assert(offsetof(struct thread_context, pc) == THREAD_CONTEXT_PC,
	      "THREAD_CONTEXT_PC mismatch");
static_assert(offsetof(struct thread_context, lr) == THREAD_CONTEXT_LR,
	      "THREAD_CONTEXT_LR mismatch");
static_assert(offsetof(struct thread_context, sp) == THREAD_CONTEXT_SP,
	      "THREAD_CONTEXT_SP mismatch");
static_assert(offsetof(struct thread_context, cpsr) == THREAD_CONTEXT_CPSR,
	      "THREAD_CONTEXT_CPSR mismatch");

#endif // __ASSEMBLER__

#endif // THREAD_H
