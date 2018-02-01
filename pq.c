#include <stddef.h> /* for size_t */
#include <stdlib.h> /* for malloc */
#include <assert.h> /* for assert */

#include "pq.h"
#include "srt_list.h"


struct pq 
{
	srt_list_t * srt_list;
};

/*******************************************************************************
PQCreate() - creates a priority queue and returns a pointer to it.
*******************************************************************************/
pq_t *PQCreate (void *params, int(*is_before)(const void *data1,
											  const void *data2,
											  void *params))
{
	pq_t *res = NULL;

	assert(is_before != NULL);

	res = (pq_t *)malloc(sizeof(*res));
	if (NULL == res)
	{
		return (NULL);
	}

	res->srt_list = SrtListCreate(params, is_before);
	if (NULL == res->srt_list)
	{
		free(res); res = NULL;
		return (NULL);
	}

	return (res);
}

/*******************************************************************************
PQDestroy() - frees all nodes in a pq.

Time complexity: O(1).
*******************************************************************************/		
void PQDestroy(pq_t *pq)
{
	assert(pq != NULL);
	assert(pq->srt_list != NULL);

	SrtListDestroy((srt_list_t *)pq->srt_list);	

	free(pq); pq = NULL;
}						

/*******************************************************************************
PQSize() - return the num of elements held in pq.

Time complexity: O(n).
*******************************************************************************/
size_t PQSize(const pq_t *pq)
{
	assert(pq != NULL);
	assert(pq->srt_list != NULL);
	
	return (SrtListSize((srt_list_t *)pq->srt_list));								
}				
	
/*******************************************************************************
PQIsempty() - return 1 if queue is empty, or 0 otherwise.

Time complexity: O(1).
*******************************************************************************/
int PQIsempty(const pq_t *pq)
{
	assert(pq != NULL);
	assert(pq->srt_list != NULL);
	
	return (SrtListIsEmpty((srt_list_t *)pq->srt_list));								
}	
	
	
/*******************************************************************************
PQEnqueue() - Enqueue a new element according to its priority into the queue.
			- returns 0 on sucess or 1 on failure

Time complexity: O(n).
*******************************************************************************/
int PQEnqueue(pq_t *pq, void *data)
{
	assert(pq != NULL);
	assert(pq->srt_list != NULL);
	
	return ((NULL == SrtListInsert((srt_list_t *)pq->srt_list, data).info));									
}		

/*******************************************************************************
PQDequeue() - removes the next element form the queue and returns its data.

Time complexity: O(1).
*******************************************************************************/
void *PQDequeue(pq_t *pq)
{
	assert(pq != NULL);
	assert(pq->srt_list != NULL);	
	
	return (SrtListPopFront(pq->srt_list)) ;								
}


/*******************************************************************************
PQPeek() - Returns the next element's data.

Time complexity: O(1).
*******************************************************************************/
void *PQPeek(pq_t *pq)
{						
	assert(pq != NULL);
	assert(pq->srt_list != NULL);
	
	return (SrtListGetData(SrtListBegin(pq->srt_list)));								
}	
	
	
	
/*******************************************************************************
PQClear() - Clears all elements from the queue.

Time complexity: O(n).
*******************************************************************************/	
void PQClear(pq_t *pq)
{	
	assert(pq != NULL);
	assert(pq->srt_list != NULL);
	
	while (0 == SrtListIsSameIter(SrtListBegin(pq->srt_list), 
								  SrtListEnd(pq->srt_list)))
	{
		PQDequeue(pq);
	}	
}	

/*******************************************************************************
PQRemove() - find a spesific element according to params, and return its data.
			 If didn't find anything returns NULL.

Time complexity: O(n).
*******************************************************************************/
void *PQRemove(pq_t *pq, const void *to_find, void *params,											
				int (*is_match)(const void *data,									
				const void *to_find,					
				void *params))
{
	srt_list_iter_t to_remove =  {0};
	void *data = NULL;	
	
	assert(pq != NULL);
	assert(pq->srt_list != NULL);
	assert(is_match != NULL);

	/* search "to_find" element in the queue */
	to_remove = SrtListFindIf(SrtListBegin(pq->srt_list),
							  SrtListEnd(pq->srt_list),	
							  to_find, 
							  params, 
							  is_match);	



	if (0 == (SrtListIsSameIter(to_remove, SrtListEnd(pq->srt_list))))
	{
		/* get data of "to_find" element */
		data = SrtListGetData(to_remove);									
		SrtListRemove(to_remove);									
	}

	return (data);
}
		
								
								

								
								
