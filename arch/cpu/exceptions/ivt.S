
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

.macro exception_trampoline handler, offset
  sub lr, lr, #\offset
  push {r0-r12, lr}
  mov r0, sp
  bl \handler
  pop {r0-r12, lr}
  add lr, lr, #4
  movs pc, lr
.endm

_svc_handler:
  exception_trampoline handle_svc, 4

_undef_handler:
  exception_trampoline handle_undefined_instruction, 4

_prefetch_abort_handler:
  exception_trampoline handle_prefetch_abort, 4

_data_abort_handler:
  exception_trampoline handle_data_abort, 8

_irq_handler:
  exception_trampoline handle_irq, 8

_fiq_handler:
  exception_trampoline handle_fiq, 8
