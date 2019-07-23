.section ".init"
.align 4

.macro TRAMPOLINE_DECLARE name
	.global rst_trampoline_\name
	.type rst_trampoline_\name, %function
	.align 4
rst_trampoline_\name:
.endm

.macro TRAMPOLINE_R0_RESULT name
	TRAMPOLINE_DECLARE \name
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
TRAMPOLINE_R0_RESULT rst_link_SwitchToZoraFastSwim

TRAMPOLINE_DECLARE rst_MoveScheduledNpcHook
  mov r4, r1 @ original instruction
  push {r0-r12, lr}
  vpush {s1-s15}
  vpush {s16-s31}
  # returns new speed in s0
  bl rst_MoveScheduledNpcHook
  vpop {s16-s31}
  vpop {s1-s15}
  pop {r0-r12, pc}

TRAMPOLINE_DECLARE rst_ui_items_IsItemAssignRequested
  push {r0-r12, lr}
  vpush {d0-d15}
  bl rst_ui_items_IsItemAssignRequested
  cmp r0, #0
  vpop {d0-d15}
  pop {r0-r12, pc}

TRAMPOLINE_DECLARE rst_ui_items_GetItemAssignIndex
  push {r0-r2, r4-r12, lr}
  vpush {d0-d15}
  bl rst_ui_items_GetItemAssignIndex
  mov r3, r0
  vpop {d0-d15}
  pop {r0-r2, r4-r12, pc}
