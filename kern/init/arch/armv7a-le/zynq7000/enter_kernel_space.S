.globl entery_kernel_space

.text
.arm

entery_kernel_space:
/*  switch mode, from svc to sys */
    MRS r0, cpsr
    AND r0, r0, #0xFFFFFFE0
    ORR r0, r0, #0b11111
    MSR cpsr, r0
/*  new stack */
	MOV	sp, #0xDFFFFFF0
	MOVS	fp, sp
/*  jump to high address */
    dsb
    isb
	BL	kinit
