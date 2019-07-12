.text
.align 4

.macro TRAMPOLINE_R0_RESULT name
	.global rst_trampoline_\name
	.type rst_trampoline_\name, %function
	.align 4
rst_trampoline_\name:
  push {r1-r12, lr}
  vpush {d0-d15}
  bl \name
  vpop {d0-d15}
  pop {r1-r12, pc}
.endm

.global rst_dummy
rst_dummy:
  nop

TRAMPOLINE_R0_RESULT rst_link_ShouldUseZoraFastSwim

