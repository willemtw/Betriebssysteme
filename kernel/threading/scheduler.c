#include <kernel/threading/scheduler.h>
#include <arch/cpu/interrupts.h>
#include <arch/cpu/registers.h>
#include <kernel/result.h>
#include <lib/kprintf.h>
#include <lib/list.h>

struct thread_queue_entry {
	list_node      node;
	struct thread *thread;
};

struct thread  scheduler_threads[NUM_THREADS];
struct thread *running_thread;

// As long as this is false, we don't try to context-switch.
// A timer interrupt could theoretically fire before the scheduler has been initialized,
// and that would break things otherwise.
static bool is_running = false;

struct thread idle_thread;

struct thread_queue_entry queue_entries[NUM_THREADS];
list_create(ready_queue);

#define container_of(ptr, type, member) ((type *)((uint8_t *)(ptr) - offsetof(type, member)))

static void scheduler_save_thread_context_from_irq(struct saved_registers *sp,
						   struct thread_context  *context)
{
	struct psr spsr = read_spsr();

	*context = (struct thread_context){
		.r0  = sp->r0,
		.r1  = sp->r1,
		.r2  = sp->r2,
		.r3  = sp->r3,
		.r4  = sp->r4,
		.r5  = sp->r5,
		.r6  = sp->r6,
		.r7  = sp->r7,
		.r8  = sp->r8,
		.r9  = sp->r9,
		.r10 = sp->r10,
		.r11 = sp->r11,
		.r12 = sp->r12,
		// Exception keeps this at the last executed instruction - we need the next one.
		.pc = sp->pc + 4,
		// The stack frame doesn't include these registers
		.lr   = read_lr_mode(spsr.d.mode),
		.sp   = read_sp_mode(spsr.d.mode),
		.cpsr = spsr,
	};
}

// Will perform the context switch by replacing saved registers/SPSR with the new
// context and letting the ASM trampoline perform the usual exception return to the new thread
static void scheduler_replace_irq_context(struct saved_registers *sp, struct thread *next_thread)
{
	struct thread_context *context = &next_thread->context;

	*sp = (struct saved_registers){
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

	queue_entries[thread_id].thread = thread;
	list_add_last(ready_queue, &queue_entries[thread_id].node);
}

void (*get_current_thread_fn(void))(void *)
{
	return running_thread->fn;
}

struct thread_context *get_current_thread_context(void)
{
	return &running_thread->context;
}

static struct thread *scheduler_get_next_thread(void)
{
	list_node *node = list_remove_first(ready_queue);
	if (node == NULL) {
		return &idle_thread;
	}

	struct thread_queue_entry *entry = container_of(node, struct thread_queue_entry, node);
	return entry->thread;
}

void scheduler_tick_from_irq(struct saved_registers *sp)
{
	if (!is_running)
		return;

	if (running_thread != &idle_thread) {
		list_add_last(ready_queue, &queue_entries[running_thread->id].node);
	}
	struct thread *next_thread = scheduler_get_next_thread();

	if (next_thread == running_thread) {
		return;
	}

	struct thread_context *context = &running_thread->context;

	scheduler_save_thread_context_from_irq(sp, context);

	running_thread->status = THREAD_STATUS_READY;

	next_thread->status = THREAD_STATUS_RUNNING;
	running_thread	    = next_thread;

	scheduler_replace_irq_context(sp, running_thread);
}

// Runs a thread immediately and yields control to it
[[noreturn]] void scheduler_run_thread(struct thread *thread)
{
	running_thread = thread;

	thread_run(thread);
}

void scheduler_thread_terminate_running_from_irq(struct saved_registers *sp)
{
	running_thread->status = THREAD_STATUS_TERMINATED;

	running_thread	       = scheduler_get_next_thread();
	running_thread->status = THREAD_STATUS_RUNNING;
	scheduler_replace_irq_context(sp, running_thread);
	kprintf("\n");
}

static void idle_thread_fn(void *arg)
{
	(void)arg;
	while (1) {
		asm volatile("wfi");
		// For local testing, busy waiting is a lot more consistent
		//for (volatile size_t i = 0; i < 10000; i++) {
		//}
	}
}

[[noreturn]] void scheduler_start(void)
{
	thread_init(&idle_thread, idle_thread_fn, NULL, 0);
	idle_thread.id = 100;

	is_running = true;
	scheduler_run_thread(scheduler_get_next_thread());

	while (1)
		;
}
