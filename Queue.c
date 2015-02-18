/* Thomas Grove
 * 0597569
 * Queue Code
 * - The Queue maintains the nodes in "Head Soon -> Latter Tail"
 */

#include "Queue.h"


/* ===========  Node  =========== */
/* Creates a new node for the queue. */
Node * newNode(void *data){
	Node *ths;
	
	/* Allocating memory */
	ths = (Node*)calloc(1,sizeof(Node));
	
	/* Setting variables */
	ths->data = data;
	ths->next = NULL;
	return ths;
}


/* ===========  Queue  =========== */
/*Adds an Node to the Queue. */
Queue * addToQueue(Queue *ths, void *e){
	Node *current;

	current = newNode(e);

	/* node and the Queue must exist*/
	if(ths == NULL || current == NULL){
		return NULL;
	}	

	/* List is empty, new Node becomes head and end. */
	if(ths->head == NULL){
		ths->head = current;
		ths->end = current;
		
	}else{
		/* Add to the End */
		ths->end->next = current;
		ths->end = current;
	}
		
	ths->size++;
	
	return ths;
}

/* Allows you to look at a node in the queue without removing it (starts at 0) (like a list) */
void * lookAtNode(Queue *ths, int ind){
	int i;

	Node *current;

	/* Queue must exist, index must be  */
	if(ths == NULL || ths->size < ind){
		return NULL;
	}	

	current = ths->head;

	for(i = 0; i < ind; i++){
		current = current->next;
	}

	return current->data;
}

/* Remmoves and returns the data in the Node at the head of the Queue. */
void * deleteNextNode(Queue *ths){
	Node *current;
	
	/* List does not exist or list is empty */
	if(ths == NULL || ths->head == NULL){
		return NULL;
	}
	
	/* Queue is only 1 long */
	if( ths->size == 1){
			current = ths->head;
			
			ths->head = NULL;
			ths->end = NULL;
			ths->size--;
	
	/* More than 1 node */
	}else{
		current = ths->head;
		
		ths->head = ths->head->next;
		ths->size--;
	}
	
	return current->data;
}

/* Creats a new Event Queue and returns a pointer to it. */
Queue * newQueue(){
	Queue *ths;
	
	ths = calloc(1, sizeof(Queue));
	ths->size = 0;
	ths->head = NULL;
	ths->end = NULL;
	return ths;
}

