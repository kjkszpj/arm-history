#AIMv6: A Toy Operating System Kernel On ARM
####Haoming Xing SID: 13307130284

##Introduction
AIMv6 is the final Programming Assignment of our Operating Course this semester. In this project we were told to implement our own operating system kernel on ARMv7a platform. Since we haven't complete all its features by now, in this report, we're going to take a review of what we have done during this semester, and introduce the feature and functionality of our operating system.
   
##Section 1. Boot System
###1.1 Small Bootloader in MBR
####1.1.1 disk partition
The SD-card storage space are partitioned into 4 partitions.  

+ 1st Partition: store the firmware(and serve as a usual sd-card).
+ 2nd Partition: has no file system, a ELF32 format file containing all kernel programs is located at head of this partition.
+ 3rd Partition: used in our self-implemented filesystem.
+ 4th Partition: used to store user programs when file system is not completed.
####1.1.2 load kernel programs
A load-elf program is positioned at MBR code, and it uses sd_dma_spin_read() method to load the kernel programs into memory at right address, and jump to the true init procedure.
###1.2 init(@low address)
Our design partitioned the virtual memory space as 2G/2G, and the kernel space is located above 2G, thus in order to do init work in kernel space, we first enabled mmu, invalidate TLB, and jumped to a predefined high_entry() which is loaded at kernel memory space according to the ELF file.
###1.3 init(@high address)
In the kinit() procedure, we start to load kernel components:

+ delete the user space's mapping from TTB, and invalidate TLB.
+ enable L1Cache
+ enable SCU
+ initialize kernel memory manage(page tables and slab)
+ enable interrupts
+ initialize scheduler
+ load file system from disk
##Section 2. Memory Management
We used one level page table for kernel memory management and two levels page table in user memory management. 
###2.1 kernel memory management
We used first-fit algorithm to support kernel memory allocation
For small object allocation we also used a slab.  
####2.1.1 page management implementation:  
A linked-list is used to represent the conscutive clean pages, each node in the list represents a consecutive blocks of memory pages.  
For allocation, first search from head until find a block larger than request size, 
and split a block that can fit into the request size from the larger block,
and add remain block to the list.
For memory free, search across the list and add the memory block back to the right position, and combine consecutive blocks when necessary.

####2.1.2 slab implementation:
Slab is implemented as many list of pools, arranged by a linked-list.
Each slab has a different (object\_size, object\_aligh) pair. 
When allocating some object, first find the slb entry(if not exist, create one),
then find a not full pool and get one object from that pool, if there is no such pool, allocate a block of memory of size(X * 10 + 10) and create a pool using that memory.
Freeing objects is similar to ways in page management.
To make thing simpler, all kmalloc() requests and slb_alloc() are served with slab in our program, and page block list is more likely to be a basement datastructure used by slab.

###2.2 user memory management
Two-level page table is(and must be) used to save memory.
In userspace memory management, each process has a different page table. When context switch happens, we have to replace the translate table.  
Our implementation provided two methods mmap() and unmmap() to handle this situation.
In case of fork() system call, we need to copy the parent process's page table to the child process, we didn't utilize the copy-on-write technique. In our implementation we simply copy the entire page table, traverse through the page table and copy the values of used pages to the addresses where the new page table was mapped to.   

##Section 3. Event Handling
The procedure of our OS event handling is described below:
+ When an interrupt arises, switch to priviliged CPU mode.
+ Store the unbanked register and change cpsr, this procedure is done by assembly codes.
+ Store the remaining CPU context correctly.
+ Then svc or irq handler is called, in other exception, the system will panic immediately.
+ On return, load the stored user context, and the assembly code will change back cpsr automatically when pc was updated. 

##Section 4. Process&Scheduler
A process in system is represented by Process Control Blocks(PCB). In the kernel's view, the processes are a bunch of PCBs arranged into many queues according to their process status.

When a scheduler is called(sched_main() in our code) work is to pick some process, load its page table and cpu context, and then run it.

###4.1 process control blocks
In our implementation, a PCB contains:  

+ process ID and process name;
+ process status (one of {NEW, READY, RUNNING, WAIT, ZOMBIE, ABORT} )
+ process return value(for already exited processes)
+ page table entry
+ CPU context(r0-r12, cpsr, spsr, sp, lr, pc, etc.)
+ scheduler info(infomation like priority, not used yet)
+ Other information (like execution time)
###4.2 process queue
To support more different kind of scheduling strategies, our design is to seprate the process data structure and scheduling mechanism.
In our implementation, there is a queue for each process status. Processes stored in the same queue have the same status. When context switch happens, we pop the running process from RUNNING queue, pick a process from READY queue and set it to RUNNING state.

###4.3 Scheduling Algorithm
We used Round-Robin algorithm in our scheduler implementation. When the CPU receive a IRQ from timer, it will call the scheduler program, and the later one will stop all running process and redispatch them.   
After a process ends, it will call an "SVC 0x9"(exit). The system will change it's status to ZOMBIE and wake up its parent process(when the parent process is blocked by wait()).

##Section 5. System Calls
###5.1 process-related system calls
####5.1.1 fork
In performing fork operation, we need to create a child process with the same context. First we choose a unused PID, copy the calling process's page table using mmap(), and create a PCB with the same content(except pid etc.) as calling process's. Finally put the child process into the ready queue. As for return value, we return the pid of new child process to the calling process, and return 0 to the child process.

####5.1.2 exec
In this system call, we erase the memory mapping in user space and load the to-be-executed ELF file in same way as bootloader does. When reading the ELF-file, we create a page table for the new process at the same time. After building the page table, we replace the CPU context and PCB of the old process, and put it into the READY queue. 

####5.1.3 exit
pop out the calling process from RUNNING queue and set its status to ZOMBIE.  
After doing this, the system will check if the process's parent process status, and wake the parent process up in case that it is blocked by waiting().
####5.1.4 wait
On this system call, the system will check the ZOMBIE queue. If there is a process with ppid equals to the calling process, the system will free that zombie process and return. In other case the calling process will fall into WAIT state and will be waken up when its child process ends.
####5.1.5 getpid
Since the pid is store in the PCB, just return the calling process's pid.
####5.1.6 getppid
Almost the same as getpid().
###5.2 IO-related system calls
Currently support　system calls　on standard I/O and file I/O.
####5.2.1 standard system calls: gets() and puts()
We extend the uart\_spin\_getbyte() method provided by TA to uart\_spin\_gets(), and use this two functions to support standard I/O system call.
####5.2.2 file IO system calls
Will be described below, at the File System part.

##Section 6. File System
We implemented a simple file system using chain-based allocation. 
All file are described as a linked list of disk blocks, which supports creating, deleting, and read/write data. 
The data structures are stored in memory and periodically checkpointed to disk.   
The kernel can call checkpoint\_fs() and load\_fs() method to checkpoint the data structure to disk.
When the sd-card is used for the first time, the kernel can call build_fs() to build a new file system on disk. 
Provides the following system calls:  

	int open(char *name, int nbyte);
open a file by given file name stored in name[] which have at most nbyte length long.  
return file descriptor on success, -1 on file not found. 

	int close(int fd);
close a file by given fd.
return 0 on success, -1 on file not found, -2 on permission denied.

	int read(int fd, char *buf, int nbyte);
read n bytes from file fd and store in buf[].
return 0 on success, -1 on file not found or permission denied.
if the file are shorter than nbytes, proceed till eof. 

	int write(int fd, char *buf, int nbyte);
append to file fd n byte stored in buf[]
return 0 on success, -1 on file not found or permission denied.
currently only allow nbytes to be a multiple of 512(block size)
 
	int create(char *name, int nbyte);
create a file named name[0\.\.nbyte-1].
return the fd on success, -1 on file already exists.

	int delete(int fd);
delete a file fd.
return 0 on success, -1 on file not found or permission denied.

##Section 7. Other staff
We solved the cross page issue on sd\_dma\_spin\_read() by handled the interrupt.
we used the private timer as our timer for scheduler. It will count down periodically from certain value then send irq.
In order to reduce difficulty of debugging and coding, we implemented a printf() function and integer division. 

##Contribution
In this project I cooperated with Peijie You and Xiaotao Liang. The work I do in this project can be divide to two parts. The first part is written in the early time and contains codes about the guide1 and guide2. The second part of my work is done recently. Since our early coded are quite different and can't be merged into a larger one, we choose to build our work on Peijie You's codebase. My main work in this operating system is to create a File System and add all I/O related system calls, and handle other small issues like cross page read problem in dma driver.
