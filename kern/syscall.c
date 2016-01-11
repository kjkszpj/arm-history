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
#include <kern/mm/pte.h>

//  todo, no use pcb_running

// for temp debug
static void load_elf_debug();

int syscall(int id)
{
    uart_spin_printf("SVC id:\t%d.\r\n\0", id);
    switch (id)
    {
        case ID_DEBUG:
            load_elf_debug();
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
    //  we will always return the FATHER
    pcb_t* c_pcb = new_pcb();
    u32 cpid = c_pcb->td.pid;
    pcb_t* f_pcb = sched_get_running();
    memcpy(c_pcb, f_pcb, sizeof(pcb_t));
    c_pcb->page_table = slb_alloc_align(PTE_L1SIZE, PTE_L1ALIGN);
//  since there is no copy on write, just copy it, the whole image.
    copy_mem_img((u32*)c_pcb->page_table, (u32*)f_pcb->page_table, 0, 0xFFFFFFFF, 0b0111100001, 0b010000111110);
    c_pcb->td.pid = cpid;
    c_pcb->td.ppid = sched_get_running()->td.pid;
    sched_mature(c_pcb);
    //  pseudo return value?
    context_svc->r0 = c_pcb->td.pid;
}

void _exec()
{
//    todo, stack for them
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
    u32 lpsr = task->cpu.cpsr;
    unmmap((u32*)task->page_table, 0, KERNEL_BASE);
    /*
     * todo, change follow from sd_spin_read to fs read
     * I copy this code form bootmain.c and elf.h, hoping there is no bug.
     */

    u8* buffer = kmalloc(BLOCK_SIZE * 5);
    sd_dma_spin_read(V2P((u32)buffer), 3, start_block);

    elf32hdr_t e = *(elf32hdr_t*)buffer;
    u32 program_entry = e.e_entry;
    u32 phoff = e.e_phoff;
    u32 phnum = e.e_phnum;
    u32 phentsize = e.e_phentsize;

    elf32_phdr_t ph;
    for (i = 0; i < phnum; i++)
    {
        sd_dma_spin_read(V2P((u32)buffer), 2, start_block + phoff / BLOCK_SIZE);
        ph = *(elf32_phdr_t*)(buffer + (phoff % BLOCK_SIZE));
        if (ph.p_type != PT_LOAD)
        {
            uart_spin_printf("type for program header not match!\r\n\0");
            continue;
        }
        u32 phe_offset = ph.p_offset;
        u32 phe_vaddr = ph.p_vaddr;
        u32 phe_memsz = ph.p_memsz;
        //  no p_flags / p_align implemented
        u8 offset = phe_offset % BLOCK_SIZE;
        u32 start_id = phe_offset / BLOCK_SIZE;
        u32 end_id = ((phe_offset + (u32)phe_memsz - 1) / BLOCK_SIZE) + 1;
        //  check the domain, and pattern
        uart_spin_printf("---cp3?---\r\n\0");
        mmap((u32*)task->page_table, (phe_vaddr >> 12 << 12), phe_vaddr + phe_memsz, 0b0111100001, 0b010000111110);
        uart_spin_printf("offset:  %d\t, to vaddr:  %d\t\r\n\0", phe_offset, phe_vaddr);
        sd_dma_spin_load(phe_vaddr - offset, end_id - start_id, start_id + start_block, (u32*)task->page_table);
        phoff += phentsize;
    }
    uart_spin_printf("Loading done, ready to execute\r\n\0");
//    memset(task->cpu, 0, sizeof(context_cpu_t));
//    todo, the program will return to nurser
//    task->cpu.lr = ;
//    task->cpu.sp = ;
//    task->cpu.fp = ;
    lpsr = ((lpsr & 0x0FFFFFE0) | 0b10000);
    task->cpu.cpsr = lpsr;
    task->cpu.spsr = lpsr;
    task->cpu.pc = program_entry;
    context_switch(task, task, context_svc);
}






void load_elf_debug()
{
    uart_spin_printf("Are you ok?");
}