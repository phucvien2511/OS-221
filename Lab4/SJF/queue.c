
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
	temp->data = proc;
	pthread_mutex_lock(&(q->lock));
	if (q->head == NULL) {
		q->tail = temp;
		q->head = temp;
	}
	else {
		struct qitem_t *ptr = q->head;
		while (ptr->next) {
			if (ptr->next->data->priority > proc->priority) break;
			ptr = ptr->next;
		}
		if (ptr->next == NULL && ptr != q->head) { //add in tail
				q->tail->next = temp;
				q->tail = q->tail->next;
		}
		else if (ptr == q->head) {
			if (ptr->data->priority <= proc->priority) {
				temp->next = ptr->next;
				ptr->next = temp;
			}
			else {
				temp->next = q->head;
				q->head = temp;
			}
		}
		else {
			temp->next = ptr->next;
			ptr->next = temp;
		}
	}
	pthread_mutex_unlock(&(q->lock));
}


