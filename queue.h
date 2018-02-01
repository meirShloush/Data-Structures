#ifndef QUEUE_H_
#define QUEUE_H_

#include <stddef.h> /* size_t */

typedef struct queue queue_t;

/* returns pointer to new queue or NULL on faliure */
queue_t *QueueCreate (void);

void QueueDestroy (queue_t *queue);

size_t QueueSize(const queue_t *queue);

int QueueIsEmpty (const queue_t *queue);

/* push element to the end of the queue
returns 0 on sucess or 1 on failure */
int QueueEnqueue(queue_t *queue, void *data);

/* removes next element
returns removed element */
void *QueueDequeue(queue_t *queue);

/* returns first elment in the queue */
void *QueuePeek(queue_t *queue);

/* append all the elments inside src to the end of the dest
src become empty */
void QueueAppend(queue_t *queue_dest, queue_t *queue_src);

#endif /* QUEUE_H_ */
