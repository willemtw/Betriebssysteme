
.section .text

.global _ivt
.balign 64
_ivt:
  b _start /* Reset handler - just _start for now */
  ldr pc, _undef_handler
  ldr pc, =_svc_handler
  ldr pc, _prefetch_abort_handler
  ldr pc, _data_abort_handler
  nop /* Reserved */
  ldr pc, _irq_handler
  ldr pc, _fiq_handler


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
  bl handle_undef
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
