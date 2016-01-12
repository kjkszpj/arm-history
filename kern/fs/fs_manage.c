#include <sys/types.h>
#include <kern/mm/slb.h>
#include <kern/mm/pages_manage.h>
#include <drivers/sd/sd-zynq7000.h>
#include <kern/fs/fs_manage.h>
#include <kern/mm/kmemory_manage.h>
//TODO locks with atomic operations

//load_fs()
//load on-disk information and resume datastructure
datablock_t *free_head, *db_table;
fentry_t *fentry_head, *fe_table;
void load_fs() {
	int i;
	int blocks = FS_DATA_SIZE >> 9;
	u32 va = (u32)kmalloc(FS_DATA_SIZE);
	void *addr = (void *)va;
	u32 pa = V2P(va);
	db_table = (datablock_t *)kmalloc(((STORAGE_SIZE) >> 9) * (sizeof (datablock_t)));
	fe_table = (fentry_t *)kmalloc(((STORAGE_SIZE) >> 9) * (sizeof (fentry_t))); //number of file entries;
	
	for (i = 0; i < (blocks >> 15); ++i) {
		if (sd_dma_spin_read(pa + (1 << 9) * (i << 15), 1 << 15, (FS_BASE >> 9) + (i << 15)) < 0) {
			uart_spin_puts("dma error loading file systems!");
			while (1);
			return ;
		}
	}

	// uart_spin_puts("phys_addr: "); puthex(pa);

	if (blocks & ((1 << 15) - 1)) {

		int remain = blocks & ((1 << 15) - 1);

		uart_spin_puts("remaining_blocks: "); puthex(remain);

		if (sd_dma_spin_read(pa + (1 << 9) * (blocks >> 15 << 15), remain, (FS_BASE >> 9) + (blocks >> 15 << 15)) < 0) {
			uart_spin_puts("dma error load file systems!");
			while (1);
			return ;
		}
	}
	
	// uart_spin_puts("dma_finish\r\n");
	
	int where = *((int*)va);
	if (where >= 0) free_head = db_table + where; else free_head = NULL; 
	va += sizeof (int);
	
	int *arr1 = (int *)va; // at most 2M datablocks(with each of 512 byte block size)
	for (i = 0; i < ((STORAGE_SIZE) >> 9); ++i) {
		db_table[i].ondisk_addr = (STORAGE_BASE + (i << 9));
		db_table[i].next_block = (arr1[i] == -1) ? 0 : (db_table + arr1[i]);
	}
	va += ((STORAGE_SIZE) >> 9) * sizeof(int);
	
	// uart_spin_puts("dbtable_finish\r\n");
	
	int cnt = *((int*)va);
	va += sizeof(int);
	
	fentry_head = fe_table;
	fentry_stored_t *arr2 = (fentry_stored_t *)va;
	for (i = 0; i < cnt; ++i) {
		fe_table[i].index_number = arr2[i].index_number;
		if (i + 1 < cnt) fe_table[i].next_entry = fe_table + i + 1;
		else fe_table[i].next_entry = NULL;
		fe_table[i].first_block = db_table + (arr2[i].addr >> 9);
		uart_spin_puts("arr2[i].addr: "); puthex(arr2[i].addr);
		uart_spin_puts("first block: "); puthex((u32)(fe_table[i].first_block));
		fe_table[i].occupied = -1;
		for (int j = 0; j < 10; ++j) fe_table[i].file_name[j] = arr2[i].file_name[j];
	}

	uart_spin_puts("file systemed loaded.\r\n\0");
	kmfree(addr);
}

int checkpoint_fs() {
	int i;
	int blocks = FS_DATA_SIZE >> 9;
	u32 va = (u32)kmalloc(FS_DATA_SIZE), pa = V2P(va);
	//uart_spin_puts("alloc#2");
	void *addr = (void *)va;
	int where = (free_head == NULL) ? -1 : (free_head - db_table);
	int *arr1 = (int *)va;
	arr1[0] = where;
	for (i = 0; i < ((STORAGE_SIZE) >> 9); ++i) {
		if (db_table[i].next_block == NULL) arr1[i + 1] = -1;
		else arr1[i + 1] = db_table[i].next_block - db_table;
	}

	va += (((STORAGE_SIZE) >> 9) + 1) * sizeof(int);

	int num_files = 0; fentry_t *x;
	for (x = fentry_head; x; x = x->next_entry) ++num_files;
	int *tx = (int *)va;
	((int*)va)[0] = num_files;
	va += sizeof (int);
	fentry_stored_t *arr2 = (fentry_stored_t *)va;
	for (x = fentry_head, i = 0; x; ++i, x = x->next_entry) {
		arr2[i].index_number = x->index_number;
		arr2[i].addr = (x->first_block->ondisk_addr - STORAGE_BASE);
		// uart_spin_puts("------\r\n\0");
		// puthex(arr2[i].addr);
		// uart_spin_puts("------\r\n\0");
		for (int j = 0; j < 10; ++j) arr2[i].file_name[j] = x->file_name[j];
	}
	for (int i = 0; i < 10; ++i) {
		puthex(tx[i]);
	}

	for (i = 0; i < (blocks >> 15); ++i) {
		if (sd_dma_spin_write(pa + (1 << 9) * (i << 15), 1 << 15, (FS_BASE >> 9) + (i << 15)) < 0) {
			uart_spin_puts("dma error checkpointing file systems to disk!");
			while (1);
			return -1;
		}
	}
	
	// uart_spin_puts("phys_addr: "); puthex(pa);
	if (blocks & ((1 << 15) - 1)) {
		int remain = blocks & ((1 << 15) - 1);
		uart_spin_puts("remaining_blocks: "); puthex(remain);
		int ret = sd_dma_spin_write(pa + (1 << 9) * (blocks >> 15 << 15), remain, (FS_BASE >> 9) + (blocks >> 15 << 15));
		if (ret < 0) {
			uart_spin_puts("dma error checkpointing file systems to disk!");
			puthex(-ret);
			while (1);
			return -1;
		}
	}
	uart_spin_puts("file systemed saved.\r\n\0");
	kmfree(addr);
	return 0;
}

//build_fs()
//clear all on-disk data and reconstruct the datastructure
void build_fs() {
	int i;
	db_table = (datablock_t *)kmalloc(((STORAGE_SIZE) >> 9) * (sizeof (datablock_t)));
	uart_spin_puts("alloc#1");
	for (i = 0; i < ((STORAGE_SIZE) >> 9); ++i) {
		db_table[i].ondisk_addr = (STORAGE_BASE + (i << 9));
		db_table[i].next_block = db_table + i + 1;
		if (i + 1 == ((STORAGE_SIZE) >> 9)) db_table[i].next_block = NULL;
	}
	free_head = db_table;
	fentry_head = NULL;	// initially no files
}

int open_file(u32 pid, u32 index_number) {
	fentry_t *x = fentry_head;
	for (; x != NULL; x = x->next_entry) {
		if (x->index_number == index_number) break ;
	}
	if (x == NULL) {
		return -1; // file not found
	}
	if (x->occupied != -1 && x->occupied != pid) return -2;
	x->occupied = pid;
	return 0;
}

int close_file(u32 pid, u32 index_number) {
	fentry_t *x = fentry_head;
	for (; x != NULL; x = x->next_entry) {
		if (x->index_number == index_number) break ;
	}
	if (x == NULL) {
		return -1; // file not found
	}
	if (x->occupied != -1 && x->occupied != pid) return -2;
	x->occupied = -1;
	return 0;
}

//int data_alloc(datablock_t *p, int num_blocks) 
//return -1 for no enough space
//return 0 for successfully allocated
datablock_t* data_alloc(int num_blocks) {
	if (num_blocks <= 0) return 0;
	datablock_t *x = free_head, *px = NULL;

	// uart_spin_puts("free_head before: "); puthex((u32)x);
	
	int i;
	for (i = 0; i < num_blocks; ++i) {
		if (x == NULL) {
			uart_spin_puts("no enough space on disk!");
			return NULL;// no enough space on disk
		}
		px = x;
		x = x->next_block;
	}
	if (px != NULL) px->next_block = NULL;
	
	
	px = free_head; free_head = x;
	
	// uart_spin_puts("free_head after: \r\n\0"); 
	// puthex((u32)p);
	// puthex((u32)free_head);
	// puthex((u32)px);
	return px;
}

int create_file(u32 index_number) {
	fentry_t *p = (fentry_t *)slb_alloc_align(sizeof (fentry_t), 0);
	p->next_entry = fentry_head;
	p->first_block = NULL;
	p->index_number = index_number;
	fentry_head = p;
	p->occupied = -1;
	return 0;
}

// WARNING: size is number of BLOCKs
// return 0: good
// return -1: no disk space
// return -2: file not found
// return -3: no permission
int append_file(u32 pid, u32 index_number, u32 addr, int size) {
	int i;
	datablock_t *p = data_alloc(size);

	// uart_spin_puts("content:"); puthex(((int*)addr)[0]);

	if (p == NULL) return -1;
	
	// puthex((u32)p);
	// puthex(p->ondisk_addr);

	// uart_spin_puts("datablocks allocated!\r\n\0");

	fentry_t *x = fentry_head;
	for (; x != NULL; x = x->next_entry) {

		// puthex(x->index_number);
		
		if (x->index_number == index_number) break ;
	}
	if (x == NULL) {
		return -2;
	}

	if (x->occupied != -1 && x->occupied != pid) return -3;
	
	// puthex((u32)(x->first_block));

	datablock_t *pt = x->first_block;
	
	if (pt != NULL) {
		while (pt->next_block != NULL) pt = pt->next_block;
		pt->next_block = p;
	} else x->first_block = p;

	for (i = 0; i < size; ++i) {
		u32 pa = V2P((addr + STORAGE_BLOCK_SIZE * i));
		
		// puthex(pa);

		if (sd_dma_spin_write(pa, 1, p->ondisk_addr >> 9) < 0) {
			uart_spin_puts("write to disk failed");
			while (1);
		}

		// int *taddr = (int *)kmalloc(STORAGE_BLOCK_SIZE);
		// puthex((u32)taddr);
		// sd_dma_spin_read(V2P((u32)taddr), 1, p->ondisk_addr >> 9);
		// puthex(taddr[0]);

		p = p->next_block;
	}

	// uart_spin_puts("append completed!\r\n\0");

	return 0;
}

void recycle_blocks(datablock_t *head) {
	datablock_t *x = head;
	for (; x != NULL; x = x->next_block) {
		x->ondisk_addr = 0;
		if (x->next_block == 0) break;
	}
	x->next_block = free_head;
	free_head = x;
}

int recycle_file(u32 pid, u32 index_number) {
	fentry_t *px = NULL, *x;
	for (x = fentry_head; x != NULL; x = x -> next_entry) {
		if (x->index_number == index_number) break ;
		px = x;
	}
	if (x == NULL) {
		//ERR NOT FOUND
		return -1;
	}
	if (x->occupied != -1 && x->occupied != pid) return -2;
	if (px != NULL) px->next_entry = x->next_entry;
	recycle_blocks(x->first_block);
	return 0;
}

datablock_t *seek_datablock(u32 pid, u32 index_number, u32 pos) { // id=0 for normal sequential read
	fentry_t *x;
	for (x = fentry_head; x != NULL; x = x -> next_entry) {
		if (x->index_number == index_number) break;
	}
	if (x == NULL) {
		return NULL; // TODO: ERR FILE NOT FOUND 
	}
	if (x->occupied != -1 && x->occupied != pid) return NULL;
	int i;
	datablock_t *p = x->first_block;

	for (i = 0; i < pos; ++i) p = p->next_block;

	return p;
}

void* read_datablock(u32 pid, u32 index_number, u32 pos, u32 size) {
	datablock_t *p = seek_datablock(pid, index_number, pos);
	
	void *ret = (void *)kmalloc(STORAGE_BLOCK_SIZE * size);
	for (int i = 0; i < size; ++i) {
		if (p == NULL) break ;
		if (sd_dma_spin_read(V2P((u32)ret + STORAGE_BLOCK_SIZE * i), 1, ((u32)(p->ondisk_addr)) >> 9) < 0) {
			uart_spin_puts("error reading datablocks on disk\r\n\0");
			return NULL;
		}
		p = p->next_block;
	}
	return ret;
} 

int fname_to_fd(char *name, int n) {
	if (n >= 10) return -1;
	fentry_t *x;
	for (x = fentry_head; x != NULL; x = x -> next_entry) {
		bool valid = true;
		for (int j = 0; j < n; ++j) if (x->file_name[j] != name[j]) {
			valid = false ; break ;
		}
		if (valid && x->file_name[n] == 0) {
			return x->index_number;
		}
	}
	return -1;
}

int create_file_from_name(char *name, int n) {
	if (n >= 10) return -1; // name too long 
	fentry_t *x;
	if (fname_to_fd(name, n) == 0) return -2;//file already exist
	int fd = 1;
	while (1) {
		bool valid = true;
		for (x = fentry_head; x != NULL; x = x -> next_entry) if (x->index_number == fd) {
			valid = false ; break ;
		}
		if (valid) break ; else ++fd;
	}
	fentry_t *p = (fentry_t *)slb_alloc_align(sizeof (fentry_t), 0);
	if (p == NULL) return -3; // no memory space
	p->next_entry = fentry_head;
	p->first_block = NULL;
	p->index_number = fd;
	for (int j = 0; j < n; ++j) p->file_name[j] = 0;
	p->file_name[n] = 0;
	fentry_head = p;
	p->occupied = -1;
	return fd;
	return -1;
}

void test_fs() {
	u32 pid = 1; // I am the kernel!
	create_file(1);
	create_file(2);
	
	// uart_spin_puts("files created completely!\r\n\0");
	
	int *x = (int *)slb_alloc_align(4, 0);	
	int *y = (int *)slb_alloc_align(4, 0);	
	*x = 23333333;
	*y = 23333334;
	
	puthex((u32)x);
	puthex((u32)y);

	puthex(*x);
	puthex(*y);
	
	int res1 = append_file(pid, 1, (u32)x, 1);
	puthex(*y);
	int res2 = append_file(pid, 2, (u32)y, 1);

	void *rx = read_datablock(pid, 1, 0, 1);
	void *ry = read_datablock(pid, 2, 0, 1);
	
	puthex((u32)rx);
	puthex((u32)ry);

	puthex(*((int*)rx));
	puthex(*((int*)ry));

	uart_spin_puts("now checkpoint and load....\r\n\0");	
	checkpoint_fs();

	load_fs();

	rx = read_datablock(pid, 1, 0, 1);
	ry = read_datablock(pid, 2, 0, 1);
	
	puthex((u32)rx);
	puthex((u32)ry);

	puthex(*((int*)rx));
	puthex(*((int*)ry));
	close_file(pid, 1);
	close_file(pid, 2);
}