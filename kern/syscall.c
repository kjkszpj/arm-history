//
// Created by Peijie You on 16/1/1.
// Modified by Xiaotao Liang
//

/*
 * system call united
 */

#include <elf.h>
#include <config.h>
#include <settings.h>
#include <interrupt.h>
#include <kern/init/init.h>
#include <kern/mm/slb.h>
#include <kern/mm/pte.h>
#include <kern/syscall.h>
#include <kern/sched/pcb.h>
#include <kern/sched/sched.h>
#include <string.h>

//  todo, no use pcb_running

// for temp debug
static void svc_print();

// store the return value at r0
int syscall(int id)
{
    uart_spin_printf("From syscall(), \r\n  SVC id:\t%d.\r\n\0", id);
    switch (id)
    {
        case ID_DEBUG:
            svc_print();
            break;
        case ID_FORK:
            _fork();
            break;
        case ID_EXEC:
            _exec();
            break;
        case ID_KILL:
            break;
        case ID_WAIT:
			_wait();
            break;
        case ID_GETPID:
			_getpid();
            break;
		case ID_GETPPID:
			_getppid();
			break;
		case ID_EXIT:
			_exit();
			break;

        //  optional?
        case ID_OPEN:
            break;
        case ID_CLOSE:
            break;
        case ID_READ:
            break;
        case ID_WRITE:
            break;
        case ID_SEEK:
            break;
        case ID_SLEEP:
            break;
        case ID_TIME:
            break;
        case ID_CLOCK:
            break;

        //  should never happen
        default:
            break;
    }
    return 0;
}

void _fork()
{
    //  we will always return the FATHER
    uart_spin_printf("hello from fork\r\n\0");
    pcb_t* c_pcb = new_pcb();
    u32 cpid = c_pcb->td.pid;
    pcb_t* f_pcb = sched_get_running();
//    uart_spin_printf("c_pcb: %x,\tf_pcb: %x\r\n\0", (u32)c_pcb, (u32)f_pcb);
    memcpy(c_pcb, f_pcb, sizeof(pcb_t));
    c_pcb->page_table = slb_alloc_align(PTE_L1SIZE, PTE_L1ALIGN);
    memset(c_pcb->page_table, 0, PTE_L1SIZE);
    //  since there is no copy on write, just copy it, the whole image.
    copy_mem_img((u32*)f_pcb->page_table, (u32*)c_pcb->page_table, 0, 0xFFFFFFFF, 0b0111100001, 0b010000100010);
    uart_spin_printf("\tcp.\r\n\0");
    c_pcb->td.pid = cpid;
    c_pcb->td.ppid = sched_get_running()->td.pid;
    sched_mature(c_pcb);
    //  pseudo return value?
    context_svc->r0 = c_pcb->td.pid;
    f_pcb->cpu.r0 = c_pcb->td.pid;
    c_pcb->cpu.r0 = 0;
    uart_spin_printf("\tcp.\r\n\0");
    uart_spin_printf("\tfather\r\n\0");
    print_pcb(f_pcb);
    uart_spin_printf("\tchild\r\n\0");
    print_pcb(c_pcb);
}

void _exec()
{
//    todo, care the STACK for them
    /*
     * procedure
     * 1.   change name?
     * 2.   change page table, release things
     * 3.   load elf
     *      a.  new pages for these memory, clear it.
     *      b.  load, piece by piece?
     * 4.   construct cpu context
     */

//    uart_spin_printf("Hello in exec\r\n\0");
//    asm volatile ("SVC 0");
    int i;
    u32 start_block = context_svc->r0;

//    uart_spin_printf("%x..\r\n\0", start_block);
    pcb_t* task = sched_get_running();
    u32 lpsr = task->cpu.cpsr;
    unmmap((u32*)task->page_table, 0, KERNEL_BASE);
    /*
     * todo, change follow from sd_spin_read to fs read
     * I copy this code form bootmain.c and elf.h, hoping there is no bug.
     */

    u8* buffer = (u8*)slb_alloc_align(BLOCK_SIZE * 5, BLOCK_SIZE);
    sd_dma_spin_read(V2P((u32)buffer), 3, start_block);

    if (buffer[1] != 'E' || buffer[2] != 'L' || buffer[3] != 'F')
    {
        uart_spin_printf("what?\r\n\0");
    }



    u32 entry = *(u32 *)(buffer + 0x18);
    u32 phoff = *((u32 *)(buffer + 0x1C));
    u32 phentsize = *((u16 *)(buffer + 0x2A));
    u32 phnum = *((u16 *)(buffer + 0x2C));

    u32 *p_header;
    for (i = 0; i < phnum; i++)
    {
        sd_dma_spin_read(V2P((u32)buffer), 2, start_block + (phoff / BLOCK_SIZE));
        p_header = (u32 *)(buffer + (phoff % BLOCK_SIZE));
        if (p_header[0] != PT_LOAD)
        {
            uart_spin_printf("type for program header not match!\r\n\0");
            continue;
        }
        u32 phe_offset = p_header[1];
        u32 phe_vaddr = p_header[2];
        u32 phe_memsz = p_header[5];
        //  no p_flags / p_align implemented
        u8 offset = phe_offset % BLOCK_SIZE;
        u32 start_id = phe_offset / BLOCK_SIZE;
        u32 end_id = ((phe_offset + (u32)phe_memsz - 1) / BLOCK_SIZE) + 1;
        //  check the domain, and pattern
        mmap((u32*)task->page_table, (phe_vaddr >> 12 << 12), phe_vaddr + (end_id - start_id) * BLOCK_SIZE, 0b0111100001, 0b010000100010);
        sd_dma_spin_load(phe_vaddr - offset, end_id - start_id, start_id + start_block, (u32*)task->page_table);
        phoff += phentsize;
    }
    slb_free_align(buffer, BLOCK_SIZE * 5, BLOCK_SIZE);

    mmap((u32*)task->page_table, KERNEL_BASE - USER_STACK, KERNEL_BASE, 0b0111100001, 0b010000100010);
    memset(&task->cpu, 0, sizeof(context_cpu_t));
    task->cpu.r0 = entry;
    lpsr = ((lpsr & 0x0FFFFFE0) | 0b10000);
    task->cpu.cpsr = lpsr;
    task->cpu.spsr = lpsr;
    task->cpu.pc = USER_ENTRY;
    task->cpu.lr = USER_ENTRY;
    memcpy(context_svc, &task->cpu, sizeof(context_cpu_t));
    context_switch(task, task, context_svc);
    print_pcb(task);

    void (*go)(void) = (void *)entry;
    uart_spin_printf("%x\r\n\0", *(u32*)(entry + 0));
    uart_spin_printf("%x\r\n\0", *(u32*)(entry + 4));
    uart_spin_printf("%x\r\n\0", *(u32*)(entry + 8));
    uart_spin_printf("------cp final------\r\n\0");
//    uart_spin_printf("%d %d\r\n\0", go, entry);
//    go();
//    uart_spin_printf("------cp-------\r\n\0");
}

void _wait()
{
    pcb_t *pcb = sched_get_running();
	while (1)
	{
		pcb_t *c_pcb = sched_get_zombie_child(pcb->td.pid);
		if (c_pcb != NULL)
		{
			sched_free(c_pcb);
			// TODO: free the memory of child process

			context_svc->r0 = c_pcb->td.ppid;
			return;
		}

		sched_block(pcb);
		// TODO: context switch
	}
}

void _exit()
{
    pcb_t *pcb = sched_get_running();
	sched_finish(pcb);
	
	// parent might be sleeping in wait()
	pcb_t *f_pcb = sched_get_bypid(pcb->td.ppid);
	if (f_pcb->status == WAIT) sched_wake(f_pcb);

	// TODO: pass orphan children to init
}

void _getpid()
{
    pcb_t* pcb = sched_get_running();
    context_svc->r0 = pcb->td.pid;
}

void _getppid()
{
    pcb_t* pcb = sched_get_running();
    context_svc->r0 = pcb->td.ppid;
}




void svc_print()
{
    uart_spin_printf("Are you ok?");
}

