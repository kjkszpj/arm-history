//
// Created by Peijie You on 16/1/1.
//

/*
 * system call united
 */

#include <elf.h>
#include <config.h>
#include <settings.h>
#include <interrupt.h>
#include <kern/syscall.h>
#include <kern/sched/pcb.h>
#include <kern/sched/sched.h>
#include <kern/init/init.h>
#include <kern/mm/kmemory_manage.h>
#include <string.h>

// for temp debug
extern void cs_debug();

int syscall(int id)
{
    uart_spin_printf("SVC id:\t%d.\r\n\0", id);
    switch (id)
    {
        case ID_DEBUG:
            cs_debug();
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
            break;
        case ID_GETPID:
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
    //  todo
    //  we will always return the FATHER
    pcb_t* c_pcb = new_pcb();
    u32 temp = c_pcb->td.pid;
    pcb_t* f_pcb = sched_get_running();
    memcpy(c_pcb, f_pcb, sizeof(pcb_t));
    c_pcb->td.pid = temp;
    c_pcb->td.ppid = sched_get_running()->td.pid;
    sched_mature(c_pcb);
    //  pseudo return value?
    context_svc->r0 = c_pcb->td.pid;
}

//  todo, how to pack the user program
void _exec()
{
    /*
     * procedure
     * 1.   change name?
     * 2.   change page table, release things
     * 3.   load elf
     *      a.  new pages for these memory, clear it.
     *      b.  load, piece by piece?
     * 4.   construct cpu context
     */

    int i;
    u32 start_block = context_svc->r0;
    pcb_t* task = sched_get_running();
    unmmap((u32*)task->page_table, 0, KERNEL_BASE);
    /*
     * todo, change follow from sd_spin_read to fs read / good read
     * I copy this code form bootmain.c, hoping there is no bug.
     */

    u8* buffer = kmalloc(BLOCK_SIZE * 3);
    sd_dma_spin_read((u32)buffer, 3, start_block);

    elf32hdr_t e = *(elf32hdr_t*)buffer;
    void (*program_entry)(void) = (void *)e.e_entry;
    u32 phoff = e.e_phoff;
    u32 phnum = e.e_phnum;
    u32 phentsize = e.e_phentsize;

    elf32_phdr_t ph;
    for (i = 0; i < phnum; i++)
    {
        sd_dma_spin_read((u32)buffer, 2, start_block + phoff / BLOCK_SIZE);
        ph = *(elf32_phdr_t*)(buffer + (phoff % BLOCK_SIZE));
        if (ph.p_type != PT_LOAD)
        {
            uart_spin_printf("type for program header not match!\r\n\0");
            continue;
        }
        u32 phe_offset = ph.p_offset;
        u32 phe_vaddr = ph.p_vaddr;
        u32 phe_memsz = ph.p_memsz;
//        no p_flags / p_align implemented
        u8 offset = phe_offset % BLOCK_SIZE;
        u32 start_id = phe_offset / BLOCK_SIZE;
        u32 end_id = ((phe_offset + (u32)phe_memsz - 1) / BLOCK_SIZE) + 1;
//        todo, pattern
//        mmap((u32*)task->page_table, (phe_vaddr >> 12 << 12), phe_vaddr + phe_memsz, , ,);
        uart_spin_printf("offset:  %d\t, to vaddr:  %d\t\r\0", phe_offset, phe_vaddr);
        // todo read
        phoff += phentsize;
    }
}