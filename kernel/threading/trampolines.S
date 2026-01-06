#include <kernel/threading/thread.h>

.section .text

.global _thread_entry

.balign 64
_thread_entry:
  /* Load thread function pointer into r1 */
  bl get_current_thread_fn 
  mov r1, r0

  /* SP points to thread arg */
  mov r0, sp
  blx r1

  /* Terminate the thread */
  mov r0, #0
  svc #0

.global _thread_run_trampoline

.balign 64
_thread_run_trampoline:
  ldr lr, [r0, #THREAD_CONTEXT_PC]
  add r0, r0, #THREAD_CONTEXT_REGS
  ldmia r0, {r0-r12}

  movs pc, lr
