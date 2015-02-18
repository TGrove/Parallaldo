/* Thomas Grove
 * 0597569
 * Queue HeaderFile
 * - Contains the Headers and declarations for all the The methods, 
 *
 */
#ifndef QUEUE_H_
#define QUEUE_H_

/* Includes */
#include <stdlib.h>
#include <stdio.h>

/* Prototype for a node */
typedef struct node_t{
	void *data;
	struct node_t *next;
}Node;

/* Prototype for the Queue */
typedef struct queue_t{
	int size;
	Node *head;
	Node *end;

}Queue;

/* Node Methods */
int deleteNode(Node *);
Node * newNode(void *);

/* Event Queue Methods */
Queue * addToQueue(Queue *, void *);
void * deleteNextNode(Queue *);
Queue * newQueue();
void * lookAtNode(Queue *ths, int ind);

#endif

