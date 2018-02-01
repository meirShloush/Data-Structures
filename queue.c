#include <assert.h> /* for assert */
#include <stdlib.h> /* for malloc */
#include <stdio.h> /* for malloc */

#include "queue.h"
#include "slist.h"


struct queue
{
	slist_node_t *head; /* points to dummy node in the queue */
	slist_node_t *tail;	/* points to the last node of the queue */
};

/*******************************************************************************
QueueCreate() - returns pointer to new queue, or NULL on faliure.
			  - mamangement_struct->head points to dummy all the time
*******************************************************************************/
queue_t *QueueCreate (void)
{	
	slist_node_t *res->head = NULL;
	
	queue_t *res = (queue_t *)malloc(sizeof(queue_t));
	if (NULL == res)
	{
		return (NULL);
	}	
	
	/* create dummy node, and initialize it to NULL */
	res->head = SListCreateAndInitNode(NULL, NULL);
	if (NULL == res->head)
	{
		return (NULL);
	}
	
	/* head and tail points to dummy */
	res->tail = res->head;
	
	return (res);
}

/*******************************************************************************
QueueDestroy() - frees all nodes in a linked list that starts at the given 
queue->head.
Time complexity: O(n).
*******************************************************************************/
void QueueDestroy (queue_t *queue)
{
	SListFreeAll(queue->head);
	
	free(queue); queue = NULL;
}

/*******************************************************************************
QueueSize() - return the num of elements held in queue.
Time complexity: O(n)
*******************************************************************************/
size_t QueueSize(const queue_t *queue)
{
	assert(SListHasLoop(queue->head) != 1);
	assert(queue != NULL);

	/* return size - dummy node */
	return ((SListSize(queue->head)) - 1);
}

/*******************************************************************************
QueueIsEmpty() - return 1 if queue is empty
Time complexity: O(1).
*******************************************************************************/
int QueueIsEmpty (const queue_t *queue)
{
	assert(queue != NULL);

	if (queue->head == queue->tail)
	{
		return (1);
	}
	
	return (0);
}

/*******************************************************************************
QueueEnqueue() push element to the end of the queue 
returns 0 on sucess or 1 on failure
Time complexity: O(1).
*******************************************************************************/
int QueueEnqueue(queue_t *queue, void *data)
{
	slist_node_t *new_tail = NULL;
	
	/* create a new node */
	slist_node_t *new_node = SListCreateAndInitNode(data, NULL);
	if (NULL == new_node)
	{
		return (1);
	}

	/* insert the new node to the tail */
	new_tail = SListInsertAfter(new_node, queue->tail);
	if (NULL == new_tail)
	{
		return (1);
	}

	/* update the management struct */
	queue->tail = new_tail;
	
	return (0);
}

/*******************************************************************************
QueueDequeue() removes next element, returns removed element
Time complexity: O(1).
*******************************************************************************/
void *QueueDequeue(queue_t *queue)
{
	void *res = NULL;
	slist_node_t *removed_node = NULL;
	
	assert(queue != NULL);

	if(QueueIsEmpty(queue))
	{
		return (NULL);
	}
	
	removed_node = SListRemoveAfter(queue->head);

	if(NULL == removed_node->next)
	{
		queue->tail = queue->head;
	}
	
	res = removed_node->data;
	
	free(removed_node);

	return (res);
 }

/*******************************************************************************
QueuePeek() - returns first elment in the queue
			- Time complexity: O(1).
*******************************************************************************/
void *QueuePeek(queue_t *queue)
{
	assert(queue != NULL);

	if (QueueIsEmpty(queue))
	{
		return(NULL);
	}
	
	return (queue->head->next->data);
}

/*******************************************************************************
QueueAppend() - append all the elments inside src to the end of the dest.
				src become empty
			  - Time complexity: O(1).
*******************************************************************************/
void QueueAppend(queue_t *queue_dest, queue_t *queue_src)
{
	assert(queue_dest != NULL);
	assert(queue_src != NULL);

	/* queue_dest->tail points to queue_src->head->next, (because queue_src->head points to dummy) */
	queue_dest->tail->next = queue_src->head->next;

	queue_dest->tail = queue_src->tail;
	
	queue_src->tail = queue_src->head;	
	
	queue_src->head->next = NULL;
}

