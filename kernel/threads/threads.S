#include <kernel/threads/scheduler.h>

.section .text

.global _thread_entry

.balign 64
_thread_entry:
  bl get_current_thread_fn /* Load thread FN pointer into r0 */
  mov r1, r0
  mov r0, sp
  /* Call the thread function*/
  blx r1
  /* Terminate the thread */
  svc 0

.global _thread_run

.balign 64
_thread_run:
  ldr lr, [r0, #THREAD_CONTEXT_PC]

  add r0, r0, #THREAD_CONTEXT_REGS
  ldmia r0, {r0-r12}

  movs pc, lr

