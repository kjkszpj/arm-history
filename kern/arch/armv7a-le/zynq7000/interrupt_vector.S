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
	b	int_pre_no
	b	int_pre_ndef
	b	int_pre_SVC
	b	int_pre_prefetch_abort
	b	int_pre_data_abort
	b	int_pre_no
	b	int_pre_IRQ
	b	int_pre_FIQ




int_pre_no:
    //  todo, better solution
    b       int_pre_no

int_pre_ndef:
    ldr     sp, =context_ndef
    ldr     sp, [sp]
    stmia   sp!, {r0-r12, lr}
    mrs     r0, spsr
    stmia   sp!, {r0}
    mrs     r0, cpsr
    and     r0, r0, #0xFFFFFFE0
    orr     r0, r0, #0b11111
    msr     cpsr, r0
    mov     r0, sp
    bl      int_ent_ndef
    mov     sp, r0
    mrs     r0, cpsr
    and     r0, r0, #0xFFFFFFE0
    orr     r0, r0, #0b11011
    msr     cpsr, r0
    ldmdb   sp!, {r0}
    msr     spsr, r0
    ldmdb   sp!, {r0-r12, pc}^


int_pre_SVC:
    /*  "stack" infact is an object */
    ldr     sp, =context_svc
    ldr     sp, [sp]

    /*
     *  dump to object via stmia, it is an object rather than stack.
     *  TODO, it does not follow the context_cpu format.
     */
    stmia   sp!, {r0-r12, lr}
    mrs     r0, spsr
    stmia   sp!, {r0}

    /*  change mode */
    mrs     r0, cpsr
    and     r0, r0, #0xFFFFFFE0
    orr     r0, r0, #0b11111
    msr     cpsr, r0

    /*
     *  since r0 have been saved, fill free to use r0 to store sp
     *  can not stmia and ldmdb sp, it will fail?
     */
    mov     r0, sp
    bl      int_ent_svc
    mov     sp, r0

    /*
     *  should first change mode for bank sp and then ldm
     *  since sp is bank, after change mode, we can not access svc_sp/context_svc, so leave it to c program to solve
     */
    mrs     r0, cpsr
    and     r0, r0, #0xFFFFFFE0
    orr     r0, r0, #0b10011
    msr     cpsr, r0

    /*  return from interrupt */
    ldmdb   sp!, {r0}
    msr     spsr, r0
    ldmdb   sp!, {r0-r12, pc}^
    /* should prevent falling? */


int_pre_prefetch_abort:
    ldr     sp, =context_abort
    ldr     sp, [sp]
    stmia   sp!, {r0-r12, lr}
    mrs     r0, spsr
    stmia   sp!, {r0}
    mrs     r0, cpsr
    and     r0, r0, #0xFFFFFFE0
    orr     r0, r0, #0b11111
    msr     cpsr, r0
    mov     r0, sp
    bl      int_ent_prefetch_abort
    mov     sp, r0
    mrs     r0, cpsr
    and     r0, r0, #0xFFFFFFE0
    orr     r0, r0, #0b10111
    msr     cpsr, r0
    ldmdb   sp!, {r0}
    msr     spsr, r0
    ldmdb   sp!, {r0-r12, pc}^


int_pre_data_abort:
    ldr     sp, =context_abort
    ldr     sp, [sp]
    stmia   sp!, {r0-r12, lr}
    mrs     r0, spsr
    stmia   sp!, {r0}
    mrs     r0, cpsr
    and     r0, r0, #0xFFFFFFE0
    orr     r0, r0, #0b11111
    msr     cpsr, r0
    mov     r0, sp
    bl      int_ent_data_abort
    mov     sp, r0
    mrs     r0, cpsr
    and     r0, r0, #0xFFFFFFE0
    orr     r0, r0, #0b10111
    msr     cpsr, r0
    ldmdb   sp!, {r0}
    msr     spsr, r0
    ldmdb   sp!, {r0-r12, pc}^


int_pre_IRQ:
    ldr     sp, =context_irq
    ldr     sp, [sp]
    stmia   sp!, {r0-r12, lr}
    mrs     r0, spsr
    stmia   sp!, {r0}
    mrs     r0, cpsr
    and     r0, r0, #0xFFFFFFE0
    orr     r0, r0, #0b11111
    msr     cpsr, r0
    mov     r0, sp
    bl      int_ent_irq
    mov     sp, r0
    mrs     r0, cpsr
    and     r0, r0, #0xFFFFFFE0
    orr     r0, r0, #0b10010
    msr     cpsr, r0
    ldmdb   sp!, {r0}
    msr     spsr, r0
    ldmdb   sp!, {r0-r12, pc}^


int_pre_FIQ:
    ldr     sp, =context_fiq
    ldr     sp, [sp]
    //  TODO, register bank, r0-r7
    stmia   sp!, {r0-r12, lr}
    mrs     r0, spsr
    stmia   sp!, {r0}
    mrs     r0, cpsr
    and     r0, r0, #0xFFFFFFE0
    orr     r0, r0, #0b11111
    msr     cpsr, r0
    mov     r0, sp
    bl      int_ent_prefetch_abort
    mov     sp, r0
    mrs     r0, cpsr
    and     r0, r0, #0xFFFFFFE0
    orr     r0, r0, #0b10111
    msr     cpsr, r0
    ldmdb   sp!, {r0}
    msr     spsr, r0
    ldmdb   sp!, {r0-r12, pc}^
