.globl entry_program

.text
.arm

/* todo, where to put it? */
/* how to refer to kernel base? */

entry_program:
	MOV	    sp, #0x7FFFFFF0
	ISB
	MOVS 	fp, sp
    ISB
    SVC     0
/* jump to program, user program should have int main() function */
    BL      main
/* on return, handcraft exit(r0) */
/* todo, check this number of ID_EXIT */
    SVC     9
/* should never continue here */
    B       loop

loop:
    B loop
