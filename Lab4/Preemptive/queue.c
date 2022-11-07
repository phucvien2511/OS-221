
#include <stdlib.h>
#include "queue.h"
#include <pthread.h>

/* Remember to initilize the queue before using it */
void initialize_queue(struct pqueue_t * q) {
	q->head = q->tail = NULL;
	pthread_mutex_init(&q->lock, NULL);
}

/* Return non-zero if the queue is empty */
int empty(struct pqueue_t * q) {
	return (q->head == NULL);
}
/* Get PCB of a process from the queue(q).
 * Return NULL if the queue is empty */
struct pcb_t * de_queue(struct pqueue_t * q) {
	struct pcb_t * proc;
	// TODO: return q->head->data and remember to update the queue's head
	// and tail if necessary. Remember to use 'lock' to avoid race
	// condition
	
	// YOUR CODE HERE
	pthread_mutex_lock(&q->lock);
	if (q->head == NULL) {
		proc = NULL;
	}else{
		struct qitem_t * temp = q->head;
		q->head = q->head->next;
		if (q->head == NULL) {
			q->tail = NULL;
		}
		else {
			q->head->prev = NULL;
		}
		proc = temp->data;
		free(temp);
	}
	pthread_mutex_unlock(&q->lock);

	return proc;
}

/* Put PCB of a process to the queue. */
void en_queue(struct pqueue_t * q, struct pcb_t * proc) {
	// TODO: Update q->tail.
	// Remember to use 'lock' to avoid race condition
	
	// YOUR CODE HERE
	struct qitem_t *temp = (struct qitem_t *)malloc(sizeof(struct qitem_t));
	temp->next = NULL;
	temp->prev = NULL;
	temp->data = proc;
	pthread_mutex_lock(&(q->lock));
	if (q->head == NULL) {
		q->tail = temp;
		q->head = temp;
	}
	else {
		if (proc->priority >= q->tail->data->priority) { //insert at end
			q->tail->next = temp;
			temp->prev = q->tail;
			q->tail = q->tail->next;
		}
		else if (proc->priority < q->head->data->priority) { //insert at head
			temp->next = q->head;
			q->head->prev = temp;
			q->head = temp;
		}
		else {
			struct qitem_t *ptr = q->head;
			while (ptr) {
				if (proc->priority >= ptr->data->priority) ptr = ptr->next;
				else break;
			}
			ptr->prev->next = temp;
			temp->prev = ptr->prev;
			temp->next = ptr;
			ptr->prev = temp;
		}
	}
	pthread_mutex_unlock(&(q->lock));
}


