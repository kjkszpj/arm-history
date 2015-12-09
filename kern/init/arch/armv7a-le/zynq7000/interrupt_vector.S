.global interrupt_vector
.text
.arm

/*
 * we trigger interrupt from non-secure SYS mode / USR mode
 * see armv7a-arm, table b1-3
 */

interrupt_vector:
/*
 * 0x00     not used / reset?
 * 0x04     undefined instruction, the cpu can not recognize the instruction
 * 0x08     SVC, enter by SVC instruction?
 * 0x0C     prefetch abort, can not fetch where the pc point at
 * 0x10     data abort
 * 0x14     not used
 * 0x18     IRQ
 * 0x1C     FIQ
 */

	b	int_pre_ndef
	b	int_pre_undefine_inst
	b	int_pre_SVC
	b	int_pre_prefetch_abort
	b	int_pre_data_abort
	b	int_pre_ndef
	b	int_pre_IRQ
	b	int_pre_FIQ

/*  should give a error message instead? */
int_pre_ndef:
    b   int_pre_ndef

int_pre_undefine_inst:
    b   int_pre_undefine_inst

int_pre_SVC:
    ldr     sp, =stack_svc
/*  dump to object via stmia, it is an object rather than stack.
 *  TODO, it does not follow the context_cpu format.
 */
    stmia   sp!, {r0-r12, lr}
    mrs     r0, cpsr
    stmia   sp!, {r0}
    mrs     r0, spsr
    stmia   sp!, {r0}

/*  change mode */
    mrs     r0, cpsr
    and     r0, r0, #0xFFFFFFE0
    orr     r0, r0, #0b11111
    msr     cpsr, r0

    bl      int_ent_svc

/*  should first change mode and then ldm*/
    mrs     r0, cpsr
    and     r0, r0, #0xFFFFFFE0
    orr     r0, r0, #0b10011
    msr     cpsr, r0

    ldmdb   sp!, {r0}
    msr     spsr, r0
    ldmdb   sp!, {r0}
    msr     cpsr, r0
    ldmdb   sp!, {r0-r12, pc}^

int_pre_prefetch_abort:
    b       int_pre_prefetch_abort

int_pre_data_abort:
    b       int_pre_data_abort

int_pre_IRQ:
    b       int_pre_IRQ

int_pre_FIQ:
    b       int_pre_FIQ