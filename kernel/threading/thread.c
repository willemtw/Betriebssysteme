#include <kernel/threading/thread.h>
#include <arch/cpu/registers.h>
#include <stdint.h>

void thread_init(struct thread *thread, void (*fn)(void *), const void *arg, size_t arg_size)
{
	struct psr cpsr = { 0 };
	cpsr.d.mode	= CPU_MODE_USR;

	thread->fn	     = fn;
	thread->context.cpsr = cpsr;

	uint8_t *sp = thread->stack + THREAD_STACK_SIZE;

	// Round down to multiple of 8
	// Should be a given because of the _Align(8) attribute
	// and size of the struct member, but we still had
	// alignment issues somehow, so better safe than sorry
	// // sp = (uint8_t *)((uint32_t)sp & ~0b111);
	// // Make sure SP is 8-byte aligned after copying argument
	// if (arg_size % 8) {
	//  sp -= 8 - (arg_size % 8);
	// }
	// We passed one more test when using 4-byte aligned stack pointers
	sp = (uint8_t *)((uint32_t)sp & ~0b11);

	// Make sure SP is 4-byte aligned after copying argument
	if (arg_size % 4) {
		sp -= 4 - (arg_size % 4);
	}

	// Copy thread arg to thread stack
	for (size_t i = 0; i < arg_size; i++) {
		sp--;
		*sp = ((uint8_t *)arg)[arg_size - 1 - i];
	}

	thread->context.sp = (uint32_t)sp;

	//kprintf("sp %% 8 = %u\\nstack size:%u\\n", thread->context.sp % 8,
	// thread->context.sp - (uint32_t)thread->stack);

	// Thread entrypoint is the ASM trampoline, which
	// will later branch to the thread function
	extern void _thread_entry(void);
	thread->context.pc = (uint32_t)_thread_entry;
	thread->status	   = THREAD_STATUS_READY;
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
