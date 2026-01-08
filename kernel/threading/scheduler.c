#include <kernel/threading/thread.h>
#include <kernel/threading/scheduler.h>
#include <arch/cpu/interrupts.h>
#include <arch/cpu/registers.h>
#include <kernel/result.h>
#include <lib/kprintf.h>
#include <lib/list.h>
#include <kernel/systick.h>
#include <lib/macros.h>

struct thread		scheduler_threads[NUM_THREADS];
struct thread	       *running_thread;
struct saved_registers *exception_frame;

list_create(run_queue);
list_create(sleep_queue);

struct thread idle_thread;

// As long as this is false, we don't try to context-switch.
// A timer interrupt could theoretically fire before the scheduler has been initialized,
// and that would break things otherwise.
static bool is_running = false;

static enum result find_free_thread_id(size_t *id)
{
	size_t next_thread_id = 0;
	while (next_thread_id < NUM_THREADS &&
	       scheduler_threads[next_thread_id].status != THREAD_STATUS_TERMINATED) {
		next_thread_id++;
	}
	if (next_thread_id >= NUM_THREADS) {
		return RESULT_ERROR;
	}

	*id = next_thread_id;
	return RESULT_OK;
}

void (*get_current_thread_fn(void))(void *)
{
	return running_thread->fn;
}

struct thread_context *get_current_thread_context(void)
{
	return &running_thread->context;
}

void scheduler_begin_exception(struct saved_registers *regs)
{
	exception_frame = regs;
}

static void scheduler_save_thread_context(struct thread_context *context)
{
	struct psr spsr = read_spsr();

	*context = (struct thread_context){
		.r0  = exception_frame->r0,
		.r1  = exception_frame->r1,
		.r2  = exception_frame->r2,
		.r3  = exception_frame->r3,
		.r4  = exception_frame->r4,
		.r5  = exception_frame->r5,
		.r6  = exception_frame->r6,
		.r7  = exception_frame->r7,
		.r8  = exception_frame->r8,
		.r9  = exception_frame->r9,
		.r10 = exception_frame->r10,
		.r11 = exception_frame->r11,
		.r12 = exception_frame->r12,
		// Exception keeps this at the last executed instruction - we need the next one.
		.pc = exception_frame->pc + 4,
		// The stack frame doesn't include these registers
		.lr   = read_lr_mode(spsr.d.mode),
		.sp   = read_sp_mode(spsr.d.mode),
		.cpsr = spsr,
	};
}

static inline void scheduler_save_current(enum thread_status status)
{
	running_thread->status = status;
	scheduler_save_thread_context(&running_thread->context);
}

// Will perform the context switch by replacing saved registers/SPSR with the new
// context and letting the ASM trampoline perform the usual exception return to the new thread
static void scheduler_replace_irq_context(struct thread *next_thread)
{
	struct thread_context *context = &next_thread->context;

	*exception_frame = (struct saved_registers){
		.r0  = context->r0,
		.r1  = context->r1,
		.r2  = context->r2,
		.r3  = context->r3,
		.r4  = context->r4,
		.r5  = context->r5,
		.r6  = context->r6,
		.r7  = context->r7,
		.r8  = context->r8,
		.r9  = context->r9,
		.r10 = context->r10,
		.r11 = context->r11,
		.r12 = context->r12,
		// We are resuming in an exception, which expects this to be at the last executed instruction.
		// We store the next instruction to execute though, so move it back 4 bytes.
		.pc = context->pc - 4,
	};

	write_lr_mode(context->cpsr.d.mode, context->lr);
	write_sp_mode(context->cpsr.d.mode, context->sp);
	write_spsr(context->cpsr);
}

void scheduler_thread_create(void (*fn)(void *), const void *arg, size_t arg_size)
{
	size_t thread_id;
	if (find_free_thread_id(&thread_id) != RESULT_OK) {
		kprintf("Could not create thread.\n");
		return;
	}

	struct thread *thread = &scheduler_threads[thread_id];
	thread->id	      = thread_id;
	thread->context	      = (struct thread_context){ 0 };

	thread_init(thread, fn, arg, arg_size);

	list_add_last(run_queue, &thread->queue);
}

static struct thread *scheduler_get_next_thread(void)
{
	list_node *node = list_remove_first(run_queue);
	if (node == NULL) {
		return &idle_thread;
	}

	return thread_from_queue(node);
}

static void scheduler_update_sleepers(void)
{
	if (list_is_empty(sleep_queue)) {
		return;
	}
	list_node *cur = sleep_queue->next;

	while (cur != sleep_queue) {
		struct thread *thread = thread_from_queue(cur);

		// Advance here because we might be modifying the list while looping
		cur = cur->next;

		if (thread->sleep_counter == 0) {
			list_remove(sleep_queue, &thread->queue);
			list_add_last(run_queue, &thread->queue);
			thread->status = THREAD_STATUS_READY;
		} else {
			thread->sleep_counter--;
		}
	}
}

static void scheduler_run_next(struct thread *next_thread)
{
	next_thread->status = THREAD_STATUS_RUNNING;
	running_thread	    = next_thread;

	scheduler_replace_irq_context(running_thread);
}

void scheduler_tick_from_irq(void)
{
	if (!is_running)
		return;

	scheduler_update_sleepers();

	if (running_thread != &idle_thread) {
		list_add_last(run_queue, &running_thread->queue);
	}
	struct thread *next_thread = scheduler_get_next_thread();

	if (next_thread == running_thread) {
		return;
	}

	scheduler_save_current(THREAD_STATUS_READY);
	scheduler_run_next(next_thread);
}

void scheduler_thread_terminate_running_from_irq(void)
{
	running_thread->status = THREAD_STATUS_TERMINATED;

	scheduler_run_next(scheduler_get_next_thread());
}

void scheduler_sleep_running_from_irq(uint32_t cycles)
{
	running_thread->sleep_counter = cycles;
	scheduler_save_current(THREAD_STATUS_SLEEPING);
	list_add_last(sleep_queue, &running_thread->queue);

	scheduler_run_next(scheduler_get_next_thread());
}

void scheduler_wait_running_from_irq(list_node *wait_queue)
{
	scheduler_save_current(THREAD_STATUS_WAITING);
	list_add_last(wait_queue, &running_thread->queue);

	scheduler_run_next(scheduler_get_next_thread());
}

void scheduler_prepare_svc_return(uint32_t value)
{
	exception_frame->r0 = value;
}

enum result scheduler_notify_one_from_irq(list_node *wait_queue, uint32_t value)
{
	struct thread *thread = thread_from_queue(list_remove_first(wait_queue));

	if (thread == NULL)
		return RESULT_ERROR;

	// INVARIANT: Running threads should not be on a wait queue
	if (thread->status == THREAD_STATUS_RUNNING) {
		kprintf("Attempted to notify running thread\n");
		while (1)
			;
	}

	// Notification value passed in r0
	thread->context.r0 = value;
	thread->status	   = THREAD_STATUS_READY;
	list_add_last(run_queue, &thread->queue);

	return RESULT_OK;
}

static void idle_thread_fn(void *arg)
{
	(void)arg;
	while (1) {
		asm volatile("wfi" ::: "memory");
	}
}

[[noreturn]] void scheduler_run_thread(struct thread *thread)
{
	running_thread = thread;

	thread_run(thread);
}

[[noreturn]] void scheduler_start(void)
{
	thread_init(&idle_thread, idle_thread_fn, NULL, 0);
	idle_thread.id = 100;

	is_running = true;
	scheduler_run_thread(scheduler_get_next_thread());
}
