#include <stddef.h> /* for size_t */	
#include <stdlib.h> /* for malloc */	
#include <assert.h> /* for assert */									

#include "srt_list.h"			
						
struct srt_list
{
	void *params;
	int (*is_before)(const void *data1,	const void *data2, void *params);
	dlist_t *dlist;
};								

	
/*******************************************************************************
SrtListCreate() - returns pointer to new srt_list, or NULL on faliure.

*******************************************************************************/
srt_list_t *SrtListCreate(void *params,	int (*is_before)(const void *data1,	
														 const void *data2,	
														 void *params))
{	
	srt_list_t *new_srt_list = NULL;
	
	assert(is_before != NULL);

	new_srt_list = (srt_list_t *)malloc(sizeof(*new_srt_list));
	if (NULL == new_srt_list)
	{
		return (NULL);
	}	

	/* Initializing fields */
	new_srt_list->params = params;
	new_srt_list->is_before = is_before;
	new_srt_list->dlist = DlistCreate();
	if (NULL == new_srt_list->dlist)
	{
		free(new_srt_list); new_srt_list = NULL;
		return (NULL);
	}
	
	return (new_srt_list);
}

/*******************************************************************************
SrtListDestroy() - frees all nodes in a srt_list that starts at the given srt_list

Time complexity: O(n).
*******************************************************************************/
void SrtListDestroy(srt_list_t *srt_list)
{
	assert(srt_list != NULL);
	assert(srt_list->dlist != NULL);

	DlistDestroy(srt_list->dlist);
	
	free(srt_list); srt_list = NULL;
}

/*******************************************************************************
SrtListSize() - return the num of elements held in srt_list.

Time complexity: O(n).
*******************************************************************************/
size_t SrtListSize(const srt_list_t *srt_list)
{
	assert(srt_list != NULL);
	assert(srt_list->dlist != NULL);

	return (DlistSize(srt_list->dlist));
}

/*******************************************************************************
SrtListIsEmpty() - returns 1 if empty; 0 if not.

Time complexity: O(1).
*******************************************************************************/
int SrtListIsEmpty(const srt_list_t *srt_list)
{
	assert(srt_list != NULL);
	assert(srt_list->dlist != NULL);

	return (DlistIsEmpty(srt_list->dlist));
}

/*******************************************************************************
SrtListInsert() - Return iter to inserted node on success, end on failure

Time complexity: O(n).
*******************************************************************************/
srt_list_iter_t SrtListInsert(srt_list_t *srt_list, void *data)
{
	srt_list_iter_t iter = {0};
	
	assert(srt_list != NULL);
	assert(srt_list->dlist != NULL);

	iter = SrtListBegin(srt_list);

	/* while the srt_list is not over, and "data" is befor "data of iter" */
	while ((0 == SrtListIsSameIter(iter, SrtListEnd(srt_list))) && 
		   (1 == srt_list->is_before(SrtListGetData(iter), data, srt_list->params)))
	{
		iter = SrtListNext(iter);
	}	
	
	iter.info = (struct srt_list_iter_info *)DlistInsert(srt_list->dlist,
														(dlist_iter_t)iter.info,
														data);
	if (NULL == iter.info)
	{
		return (iter);
	}							

	return (iter);
}

/*******************************************************************************
IsIterValid() - helper function - Return 1 if iter is valid, and 0 otherwise.

Time complexity: O(1).
*******************************************************************************/
static int IsIterValid(srt_list_iter_t iter)
{
	return (iter.info != NULL);
}

/*******************************************************************************
SrtListRemove() - Return iter to the next node.

Time complexity: O(1).
*******************************************************************************/
srt_list_iter_t SrtListRemove(srt_list_iter_t whom)
{
	srt_list_iter_t res_iter = {0};
	
	assert(1 == IsIterValid(whom));
	
	res_iter.info = (struct srt_list_iter_info *)DlistErase((dlist_iter_t)whom.info);
	
	return (res_iter);
}

/*******************************************************************************
SrtListBegin() - return the first node of srt_list

Time complexity: O(1).
*******************************************************************************/
srt_list_iter_t SrtListBegin(const srt_list_t *srt_list)
{
	srt_list_iter_t res = {0};
	
	assert(srt_list != NULL);
	assert(srt_list->dlist != NULL);
	
	res.info = (struct srt_list_iter_info *)DlistBegin(srt_list->dlist);

	return (res);
}

/*******************************************************************************
SrtListEnd() - return the last node of srt_list

Time complexity: O(1).
*******************************************************************************/
srt_list_iter_t SrtListEnd(const srt_list_t *srt_list)
{
	srt_list_iter_t res = {0};
	
	assert(srt_list != NULL);
	assert(srt_list->dlist != NULL);
	
	res.info = (struct srt_list_iter_info *)DlistEnd(srt_list->dlist);

	return (res);
}
	
/*******************************************************************************
SrtListPrev() - return the previous node of the givev current

Time complexity: O(1).
*******************************************************************************/
srt_list_iter_t SrtListPrev(srt_list_iter_t current)
{
	srt_list_iter_t res = {0};
	
	assert(1 == IsIterValid(current));
	
	res.info = (struct srt_list_iter_info *)DlistPrev((dlist_iter_t)current.info);

	return (res);
}

/*******************************************************************************
SrtListNext() - return the next node of the givev current

Time complexity: O(1).
*******************************************************************************/
srt_list_iter_t SrtListNext(srt_list_iter_t current)
{
	srt_list_iter_t res = {0};
	
	assert(1 == IsIterValid(current));

	res.info = (struct srt_list_iter_info *)DlistNext((dlist_iter_t)current.info);

	return (res);
}

/*******************************************************************************
SrtListGetData() - return the data of the givev iter

Time complexity: O(1).
*******************************************************************************/
void *SrtListGetData(srt_list_iter_t iter)
{
	assert(1 == IsIterValid(iter));
	
	return (DlistGetData((dlist_iter_t)iter.info));
}
	
/*******************************************************************************
SrtListIsSameIter() - returns 1 if iter1 and iter2 is the same, and 0 if not.

Time complexity: O(1).
*******************************************************************************/
int SrtListIsSameIter(srt_list_iter_t iter1, srt_list_iter_t iter2)
{
	assert(1 == IsIterValid(iter1));	
	assert(1 == IsIterValid(iter2));

	return (DlistIsSameIter((dlist_iter_t)iter1.info, (dlist_iter_t)iter2.info));
}

/*******************************************************************************
SrtListForEach() - iterate through the sorted list.									
					returns the value of do_func:
					if the value is a non-zero, stops iterations

Time complexity: O(n).
*******************************************************************************/
int SrtListForEach(srt_list_iter_t from,									
					srt_list_iter_t to,					
					void *params,					
					int (*do_func)(void *data, void *params))
{
	assert(1 == IsIterValid(from));
	assert(1 == IsIterValid(to));
	assert(do_func != NULL);

	return (DlistForEach((dlist_iter_t)from.info, (dlist_iter_t)to.info,
						  do_func,
						  params));
}

/*******************************************************************************
SrtListFind() - returns the iterator to the first node contains the searched data.
				returns 'to' if didn't find.
Time complexity: O(n).
*******************************************************************************/
srt_list_iter_t SrtListFind(srt_list_t *srt_list,									
							srt_list_iter_t from,		
							srt_list_iter_t to,		
							const void *to_find)
{
	assert(1 == IsIterValid(from));
	assert(1 == IsIterValid(to));
	assert(srt_list != NULL);

	while (0 == SrtListIsSameIter(from, to))
	{	   
		/* If each one is before the other, it's the same data */
		if (0 == (srt_list->is_before(SrtListGetData(from), to_find, srt_list->params)))
		{
			if (0 == (srt_list->is_before(to_find, SrtListGetData(from), srt_list->params)))
			{
				return (from);
			}
			return (to);
		}
		
		from = SrtListNext(from);	
	}
	
	return (to);
}

/*******************************************************************************
SrtListFindIf() - returns the iterator to the first node contains the searched data 
				  returns 'to' if didn't find.

Time complexity: O(n).
*******************************************************************************/
srt_list_iter_t SrtListFindIf(srt_list_iter_t from,	srt_list_iter_t to,				
						   	const void *to_find,			
							void *params,				
							int (*is_match)(const void *node_data,					
											const void *to_find,	
											void *params))
{
	assert(1 == IsIterValid(from));
	assert(1 == IsIterValid(to));
	assert(to_find != NULL);
	assert(is_match != NULL);

	while ((0 == SrtListIsSameIter(from, to)) &&
		  (0 == is_match(SrtListGetData(from), to_find, params)))
	{
		from = SrtListNext(from);
	}

	return (from);
}

/*******************************************************************************
SrtListPopBack() - pops out the last node of the list and returns its data

Time complexity: O(1).
*******************************************************************************/									
void *SrtListPopBack(srt_list_t *srt_list)
{
	assert(srt_list != NULL);	
	assert(srt_list->dlist != NULL);

	return ( DlistPopBack(srt_list->dlist));
}

/*******************************************************************************
SrtListPopFront() - pops out the first node of the list and returns its data

Time complexity: O(1).
*******************************************************************************/
void *SrtListPopFront(srt_list_t *srt_list)
{	
	assert(srt_list != NULL);
	assert(srt_list->dlist != NULL);

	return ( DlistPopFront(srt_list->dlist));
}

/*******************************************************************************
SrtListMerge() - Merges to lists into one sorted list (which is dest).
				 src becomes empty.

Time complexity: O(n1*n2).
*******************************************************************************/
void SrtListMerge(srt_list_t *dest, srt_list_t *src)
{
	assert(dest != NULL);
	assert(src != NULL);
	
	while (0 == SrtListIsEmpty(src))
	{
		SrtListInsert(dest, SrtListPopFront(src));
	}
}



