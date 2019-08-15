.section ".init"
.align 4
.global rst_OdolwaChooseChargeAttackRandomly
rst_OdolwaChooseChargeAttackRandomly:
  ldr r1, =0x3F200000 // 0.625
  cmp r0, r1
  bx lr
