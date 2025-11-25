
.section .text

.global _ivt
.balign 64
_ivt:
  ldr pc, =_start /* Reset handler - just _start for now */
  ldr pc, =_undef_handler
  ldr pc, =_svc_handler
  ldr pc, =_prefetch_abort_handler
  ldr pc, =_data_abort_handler
  nop /* Reserved */
  ldr pc, =_irq_handler
  ldr pc, =_fiq_handler

/* TODO:: Leave LR at causing instruction, decrement to following instruction after c handler  */

_svc_handler:
  /* Decrement LR due to pipelining */
  /* sub lr, lr, #4 */
  push {r0-r12, lr}
  mov r0, sp
  bl handle_svc
  pop {r0-r12, lr}
  movs pc, lr

_undef_handler:
  /* Decrement LR due to pipelining */
  /* sub lr, lr, #4 */
  push {r0-r12, lr}
  mov r0, sp
  bl handle_undefined_instruction
  pop {r0-r12, lr}
  movs pc, lr

_prefetch_abort_handler:
  /* Decrement LR due to pipelining */
  /* sub lr, lr, #4 */
  push {r0-r12, lr}
  mov r0, sp
  bl handle_prefetch_abort
  pop {r0-r12, lr}
  movs pc, lr

_data_abort_handler:
  /* Decrement LR due to pipelining */
  sub lr, lr, #4
  push {r0-r12, lr}
  mov r0, sp
  bl handle_data_abort
  pop {r0-r12, lr}
  movs pc, lr

_irq_handler:
  /* Decrement LR due to pipelining */
  sub lr, lr, #4
  push {r0-r12, lr}
  mov r0, sp
  bl handle_irq
  pop {r0-r12, lr}
  movs pc, lr

_fiq_handler:
  /* Decrement LR due to pipelining */
  sub lr, lr, #4
  push {r0-r12, lr}
  mov r0, sp
  bl handle_fiq
  pop {r0-r12, lr}
  movs pc, lr
