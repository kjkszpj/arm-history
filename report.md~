#Toy Operating System Kernel On ARM

##What We Have Done

##Phase 1. Boot System
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
Our design partitioned the virtual memory space as 2G/2G, and the kernel space is located over 2G, thus in order to do init work in kernel space, we first enabled mmu, and jumped to a predefined high_entry().
###1.3 init(@high address)
In the kinit() procedure, we start to load kernel components:

+ delete the user space's mapping from TTB, and invalidate TLB.
+ enable L1Cache
+ enable SCU
+ initialize kernel memory manage(page tables and slab)
+ enable interrupts
+ initialize scheduler
+ load file system from disk
##Phase 2. Memory Management
We used one level page table for kernel memory management and two levels page table in user memory management. 
###2.1 kernel memory management
We used first-fit algorithm to support kernel memory allocation
For small object allocation we also used a slab.  
####2.1.1 Page Management Implementation:  
A linked-list is used to represent the conscutive clean pages, each node in the list represents a consecutive blocks of memory pages.  
For allocation, first search from head until find a block larger than request size, 
and split a block that can fit into the request size from the larger block,
and add remain block to the list.
For memory free, search across the list and add the memory block back to the right position, 
and combine consecutive blocks when necessary.

####2.1.2 Slab Implementation:
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

##Phase 3. Event Handling
###3.1 Interrupt Vector

##Phase 4. Process&Scheduler

###4.1 Process Control Block
PCB 
###4.2 Process queue
###4.3 Scheduling Algorithm

##Phase 5. System Calls


##Phase 6. File System
We implemented a simple file system using chain-based allocation. 
All file are described as a linked list of disk blocks, which supports append and sequentially read/write data. 
The data structures are stored in memory and periodically checkpointed to disk.   
The kernel can call checkpoint\_fs() and load\_fs() method to checkpoint the datastructure to disk.
When the sd-card is first time to use, the kernel can call build_fs() to build a new file system on disk. 
Provides the following system calls:  

	int open(char *name, int nbyte);
open a file by given file name stored in name[] and have nbyte length long.  
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

