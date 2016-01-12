#ifndef _FSM_H
#define _FSM_H

#include <sys/types.h>
#include <kern/mm/slb.h>
#include <drivers/sd/sd-zynq7000.h>
//design:

typedef struct datablock_t {
	struct datablock_t *next_block;
	u32 ondisk_addr;
} datablock_t;

typedef struct fentry_t {
	//TODO LOCKED
	u32 index_number;
<<<<<<< HEAD
	char file_name[10];
=======
>>>>>>> a03b83f424410650b0dec7fa82a3a84ba95a7d11
	struct datablock_t *first_block;
	struct fentry_t *next_entry;
	int occupied;
} fentry_t;

typedef struct fentry_stored_t {
	u32 index_number;
	u32 addr;
<<<<<<< HEAD
	char file_name[10];
=======
>>>>>>> a03b83f424410650b0dec7fa82a3a84ba95a7d11
}fentry_stored_t;
//void *kmalloc(u32 size)
#define FS_DATA_SIZE 			0x00100000
#define STORAGE_SIZE			0x01000000
#define STORAGE_BASE 			0xC0000000
#define FS_BASE 				0xC2000000
#define STORAGE_BLOCK_SIZE 		0x00000200

void load_fs();

int checkpoint_fs();

void build_fs();

int open_file(u32 pid, u32 index_number);

int close_file(u32 pid, u32 index_number);

datablock_t* data_alloc(int num_blocks);

<<<<<<< HEAD
int create_file(u32 index_number);
=======
int create_file(u32 pid, u32 index_number);
>>>>>>> a03b83f424410650b0dec7fa82a3a84ba95a7d11

// WARN: size: number of BLOCKs
int append_file(u32 pid, u32 index_number, u32 vaddr, int size);

void recycle_blocks(datablock_t *head);

int recycle_file(u32 pid, u32 index_number);

datablock_t *seek_datablock(u32 pid, u32 index_number, u32 block_id);

void *read_datablock(u32 pid, u32 index_number, u32 block_id, u32 size);

void test_fs();

<<<<<<< HEAD
int fname_to_fd(char *name, int n);

int create_file_from_name(char *name, int n);

=======
>>>>>>> a03b83f424410650b0dec7fa82a3a84ba95a7d11
#endif