.global svcOutputDebugString
.type svcOutputDebugString, %function
svcOutputDebugString:
   str r0, [sp,#-0x4]!
   svc 0x3D
   ldr r2, [sp], #4
   str r1, [r2]
   bx lr
