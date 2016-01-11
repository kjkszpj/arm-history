/*
 * Copyright (C) 2016 Xiaotao Liang <xtliang13@fudan.edu.cn>
*/

#include <kern/sched/policy_rr.h>
#include <kern/mm/slab.h>
#include <drivers/serial/uart.h>

typedef struct queue_element
{
	pcb_t *pcb;
	struct queue_element *next;
} queue_element_t;

typedef struct queue
{
	queue_element_t *head, *tail;
} queue_t;

queue_t queue_new, queue_ready, queue_running, queue_wait, queue_done, queue_abort;
slab_cache_t queue_pool;

void sched_init()
{
	queue_new.head = queue_new.tail = NULL;
	queue_ready.head = queue_ready.tail = NULL;
	queue_running.head = queue_running.tail = NULL;
	queue_wait.head = queue_wait.tail = NULL;
	queue_done.head = queue_done.tail = NULL;
	queue_abort.head = queue_abort.tail = NULL;

	init_slab_cache(&queue_pool, sizeof(queue_element_t), 20, 2, 4);
}

void print_queue(queue_t queue)
{
	for (queue_element_t *p = queue.head; p!=NULL; p = p->next)
	{
		puthex((u32)p->pcb);
		uart_spin_puts("\r\n");
	}
	uart_spin_puts("\r\n");
}

void queue_push(queue_t *queue, pcb_t *task)
{
	queue_element_t *element = (queue_element_t *)alloc_obj(&queue_pool);
	element->pcb = task;
	element->next = NULL;
	if (queue->head == NULL)
		queue->head = queue->tail = element;
	else
		queue->tail->next = element;
}

void queue_pop(queue_t *queue, pcb_t *task)
{
	queue_element_t *last = NULL;
	for (queue_element_t *p = queue->head; p!=NULL; p = p->next)
	{
		if (p->pcb == task)
		{
			if (queue->head->pcb == task) queue->head = p->next;
			if (queue->tail->pcb == task) queue->tail = last;
			if (last != NULL) last->next = p->next;
			free_obj(&queue_pool, p);
			break;
		}
		last = p;
	}
}

void sched_new(pcb_t *task)
{
	queue_push(&queue_new, task);
	task->status = NEW;
}

void sched_mature(pcb_t *task)
{
	queue_pop(&queue_new, task);
	queue_push(&queue_ready, task);
	task->status = READY;
}

void sched_allow(pcb_t *task)
{
	queue_pop(&queue_ready, task);
	queue_push(&queue_running, task);
	task->status = RUNNING;
}

void sched_block(pcb_t *task)
{
	queue_pop(&queue_running, task);
	queue_push(&queue_wait, task);
	task->status = WAIT;
}

void sched_preempt(pcb_t *task)
{
	queue_pop(&queue_running, task);
	queue_push(&queue_ready, task);
	task->status = READY;
}

void sched_wake(pcb_t *task)
{
	queue_pop(&queue_wait, task);
	queue_push(&queue_ready, task);
	task->status = READY;
}

void sched_finish(pcb_t *task)
{
	queue_pop(&queue_running, task);
	queue_push(&queue_done, task);
	task->status = DONE;
}

void sched_kill(pcb_t *task)
{
	switch (task->status)
	{
		case NEW: queue_pop(&queue_new, task); break;
		case READY: queue_pop(&queue_ready, task); break;
		case RUNNING: queue_pop(&queue_running, task); break;
		case WAIT: queue_pop(&queue_wait, task); break;
		default: break;
	}
	queue_push(&queue_abort, task);
	task->status = ABORT;
}

pcb_t* sched_get_bypid_in(queue_t *queue, int pid)
{
	for (queue_element_t *p = queue->head; p != NULL; p = p->next)
		if (p->pcb->td.pid == pid) return p->pcb;
	return NULL;
}

pcb_t* sched_get_bypid(int pid)
{
	pcb_t *pcb;
	if ((pcb=sched_get_bypid_in(&queue_new, pid))!=NULL) return pcb;
	if ((pcb=sched_get_bypid_in(&queue_ready, pid))!=NULL) return pcb;
	if ((pcb=sched_get_bypid_in(&queue_running, pid))!=NULL) return pcb;
	if ((pcb=sched_get_bypid_in(&queue_wait, pid))!=NULL) return pcb;
	if ((pcb=sched_get_bypid_in(&queue_done, pid))!=NULL) return pcb;
	if ((pcb=sched_get_bypid_in(&queue_abort, pid))!=NULL) return pcb;
	return NULL;
}

pcb_t* sched_get_running()
{
	return queue_running.head->pcb;
}

pcb_t* sched_pick()
{
	//TODO
	return NULL;
}

