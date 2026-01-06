#include <kernel/threading/thread.h>
#include <arch/cpu/registers.h>
#include <stdint.h>
#include <string.h>
#include <lib/macros.h>

void thread_init(struct thread *thread, void (*fn)(void *), const void *arg, size_t arg_size)
{
	struct psr cpsr = { 0 };
	cpsr.d.mode	= CPU_MODE_USR;

	thread->fn	     = fn;
	thread->context.cpsr = cpsr;

	uint8_t *sp = thread->stack + THREAD_STACK_SIZE;

	sp -= arg_size;
	// Round down to multiple of 8 because:
	// - SP needs to be 8-aligned at call sites, required by AAPCS
	// - We don't know the alignment of the argument, but the maximum
	//   possible required alignment of a struct on 32-bit ARM is 8,
	//   so we can't go wrong here.
	sp = (uint8_t *)((uint32_t)sp & ~0b111);

	memcpy((uint8_t *)sp, arg, arg_size);

	thread->context.sp = (uint32_t)sp;

	// Thread entrypoint is the ASM trampoline, which
	// will later branch to the thread function
	extern void _thread_entry(void);
	thread->context.pc = (uint32_t)_thread_entry;
	thread->status	   = THREAD_STATUS_READY;
}

struct thread *thread_from_queue(list_node *node)
{
	if (node == NULL)
		return NULL;
	return container_of(node, struct thread, queue);
}

[[noreturn]] void thread_run(struct thread *thread)
{
	struct thread_context *context = &thread->context;

	write_sp_mode(context->cpsr.d.mode, context->sp);
	write_lr_mode(context->cpsr.d.mode, context->lr);
	write_spsr(context->cpsr);

	thread->status = THREAD_STATUS_RUNNING;

	extern void _thread_run_trampoline(struct thread_context * context)
		__attribute__((noreturn));
	_thread_run_trampoline(context);
}
