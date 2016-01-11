.globl entry_program

.text
.arm

/* todo, where to put it? */
/* how to refer to kernel base? */

entry_program:
/* just for sure*/
/*
	MOV	sp, #0x7FFFFFF0
	MOVS	fp, sp
    ISB*/
    mov pc, lr
/* jump to program, user program should have int main() function */
/*    BL  main */
/* on return, handcraft exit(r0) */
/* todo, check this number */
    SVC 0
/* should never continue here */
    B loop

loop:
    B loop
