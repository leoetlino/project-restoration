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

.macro TRAMPOLINE_CMP_RESULT name, val
  TRAMPOLINE_DECLARE \name
  push {r0-r12, lr}
  vpush {d0-d15}
  bl \name
  cmp r0, \val
  vpop {d0-d15}
  pop {r0-r12, pc}
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

TRAMPOLINE_CMP_RESULT rst_ui_items_IsItemAssignRequested, #0

TRAMPOLINE_DECLARE rst_ui_items_GetItemAssignIndex
  push {r0-r2, r4-r12, lr}
  vpush {d0-d15}
  bl rst_ui_items_GetItemAssignIndex
  mov r3, r0
  vpop {d0-d15}
  pop {r0-r2, r4-r12, pc}

TRAMPOLINE_DECLARE rst_HandleOcarinaSong
  push {lr}

  push {r0-r12}
  vpush {d0-d15}
  mov r1, r0 // song
  mov r0, r4 // MessageWindow* this
  bl rst_HandleOcarinaSong
  cmp r0, #0
  vpop {d0-d15}
  pop {r0-r12}

  // jump out of this trampoline and MessageWindow code directly
  addne sp, sp, #0x70
  popne {r4-r11, pc}

  cmp r0, #0x16 // original instruction
  pop {pc}

.global rst_CameraChangeModeExHook
rst_CameraChangeModeExHook:
  push {lr}
  vpush {d0-d15}
  mov r4, r0
  mov r5, r1
  mov r6, r2
  bl rst_ShouldSwitchToFreeCam
  cmp r0, #1
  moveq r5, #0x1f
  mov r0, r4
  mov r1, r5
  mov r2, r6
  add r4, r0, #0x100 // original instruction
  vpop {d0-d15}
  pop {pc}

TRAMPOLINE_CMP_RESULT rst_IsGohtCollided, #1

TRAMPOLINE_DECLARE rst_GetGyorgCollisionResponse
  push {r0-r9, r11-r12, lr}
  vpush {d0-d15}
  mov r0, r4 // this
  bl rst_GetGyorgCollisionResponse
  mov r10, r0
  vpop {d0-d15}
  pop {r0-r9, r11-r12, pc}
