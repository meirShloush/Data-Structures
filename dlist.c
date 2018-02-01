#include <stddef.h> /* for size_t */	
#include <stdlib.h> /* for malloc */	
#include <stdio.h> /* for printf */	
#include <assert.h> /* for assert */	

#include "dlist.h"

struct dlist_node
{
	void *data;
	dlist_iter_t prev;
	dlist_iter_t next;
};

struct dlist
{
	struct dlist_node head;
	struct dlist_node tail;
	
};						
							
/*******************************************************************************
DlistCreate() - returns pointer to new Dlist, or NULL on faliure.
			  - at the head, and at the tail of Dlist have dummy nodes all the time
*******************************************************************************/							
dlist_t *DlistCreate(void)
{
	dlist_t *new_dlist = (dlist_t *)malloc(sizeof(*new_dlist));
	if(NULL == new_dlist)
	{
		return (NULL);
	}
	
	/* Initializing fields */
	new_dlist->head.data = NULL;
	new_dlist->head.prev = NULL;
	new_dlist->head.next = &new_dlist->tail;
	
	new_dlist->tail.data = NULL;
	new_dlist->tail.prev = &new_dlist->head;
	new_dlist->tail.next = NULL;

	return (new_dlist);
}

/*******************************************************************************
DlistDestroy() - frees all nodes in a Dlist that starts at the given dlist.

Time complexity: O(n).
*******************************************************************************/							
void DlistDestroy(dlist_t *dlist)
{
	dlist_iter_t node = NULL;
	
	assert(dlist != NULL);
	
	node = dlist->head.next;
	
	while (node->next != NULL)
	{
		dlist_iter_t to_remove = node;

		node = node->next;

		free(to_remove); to_remove = NULL;
	}
	
	free(dlist); dlist = NULL;
}

/*******************************************************************************
DlistSize() - return the num of elements held in Dlist

Time complexity: O(n).
*******************************************************************************/						
size_t DlistSize(const dlist_t *dlist)
{
	size_t counter = 0;
	
	dlist_iter_t node = NULL;
		
	assert(dlist != NULL);

	node = dlist->head.next;
	
	/* while (!DlistIsSameIter(node, DlistBegin(dlist) )) */
	while (node->next != NULL)
	{
		++counter;
		node = node->next;
	}
	
	return (counter);
}

/*******************************************************************************
DlistIsEmpty()- returns 1 if empty; 0 if not

Time complexity: O(1).
*******************************************************************************/							
int DlistIsEmpty(const dlist_t *dlist)
{
	assert(dlist != NULL);
	
	return (dlist->head.next == &dlist->tail);
}

/*******************************************************************************
DlistBegin() - return the first node which is not dummy

Time complexity: O(1).
*******************************************************************************/							
dlist_iter_t DlistBegin(dlist_t *dlist)
{
	assert(dlist != NULL);

	return (dlist->head.next);
}

/*******************************************************************************
DlistEnd() - return the last node which is dummy. (out of range)

Time complexity: O(1).
*******************************************************************************/							
dlist_iter_t DlistEnd(dlist_t *dlist)
{
	assert(dlist != NULL);

	return (&dlist->tail);
}

/*******************************************************************************
DlistNext() - return the next node of the givev current.

Time complexity: O(1).
*******************************************************************************/							
dlist_iter_t DlistNext(dlist_iter_t current)
{
	assert(current != NULL);

	return (current->next);
}

/*******************************************************************************
DlistPrev() - return the previous node of the givev current

Time complexity: O(1).
*******************************************************************************/							
dlist_iter_t DlistPrev(dlist_iter_t current)
{
	assert(current != NULL);
	
	return (current->prev);
}

/*******************************************************************************
DlistIsSameIter() - returns 1 if iter1 and iter2 is the same, and 0 if not.

Time complexity: O(1).
*******************************************************************************/						
int DlistIsSameIter(dlist_iter_t iter1, dlist_iter_t iter2)
{
	assert(iter1 != NULL);
	assert(iter2 != NULL);

	return (iter1 == iter2);
}

/*******************************************************************************
DlistGetData() - return the data of the givev iter

Time complexity: O(1).
*******************************************************************************/						
void *DlistGetData(dlist_iter_t iter)
{
	assert(iter != NULL);

	return (iter->data);
}

/*******************************************************************************
DlistInsert() - returns iterator to inserted node on sucess
				or end iterator on faliure 

Time complexity: O(1).
*******************************************************************************/							
dlist_iter_t DlistInsert(dlist_t *dlist, dlist_iter_t where, void *data)
{
	dlist_iter_t insert = NULL;
	
	assert(dlist != NULL);
	assert(where != NULL);
	assert(where->prev != NULL);

	insert = (dlist_iter_t)malloc(sizeof (*insert));
	if (NULL == insert)
	{
		return (&dlist->tail);
	}
	
	insert->data = data;
	
	insert->prev = where->prev;
	insert->next = where;
	where->prev->next = insert;
	where->prev = insert;

	return (insert);

}

/*******************************************************************************
DlistErase()- Erase node, and returns iterator to next node in the list

Time complexity: O(1).
*******************************************************************************/						
dlist_iter_t DlistErase(dlist_iter_t whom)
{
	dlist_iter_t to_remove = whom;
	dlist_iter_t res = NULL;
		
	assert(whom != NULL);

	/* assert(whom->prev != NULL);*/  /* whom is a dummy head */
	/* assert(whom->next != NULL);*/ /* whom is a dummy tail */
	
	/* whom is a dummy head or whom is a dummy tail */
	if ((whom->prev == NULL) || (whom->next == NULL))
	{
		return (NULL);
	} 
	

	res = whom->next;
	
	whom->prev->next = whom->next;
	whom->next->prev = whom->prev;

	free(to_remove); to_remove = NULL;
	
	return (res);
}

/*******************************************************************************
DlistPushBack() - returns iterator to the new node

Time complexity: O(1).
*******************************************************************************/					
dlist_iter_t DlistPushBack(dlist_t *dlist, void *data)
{
	assert(dlist != NULL);
	
	return (DlistInsert(dlist, &dlist->tail, data));
}

/*******************************************************************************
DlistPushFront() - returns iterator to the new node

Time complexity: O(1).
*******************************************************************************/						
dlist_iter_t DlistPushFront(dlist_t *dlist, void *data)
{
	assert(dlist != NULL);
	
	return (DlistInsert(dlist, dlist->head.next, data));	
}

/*******************************************************************************
DlistPopBack() - returns data of the poped out node

Time complexity: O(1).
*******************************************************************************/
void *DlistPopBack(dlist_t *dlist)
{
	void *data = NULL;	
	dlist_iter_t to_remove = NULL;
	
	assert(dlist != NULL);

	if(DlistIsEmpty(dlist) != 1)
	{
		to_remove = dlist->tail.prev;
		data = to_remove->data;
		DlistErase(to_remove);
	}
	return (data);
}

/*******************************************************************************
DlistPopFront()- returns data of the poped out node

Time complexity: O(1).
*******************************************************************************/							
void *DlistPopFront(dlist_t *dlist)
{
	void *data = NULL;
	dlist_iter_t to_remove = NULL;
		
	assert(dlist != NULL);
	if(DlistIsEmpty(dlist) != 1)
	{
		to_remove = dlist->head.next;
		data = to_remove->data;
		DlistErase(to_remove);
	}
	return (data);	
}

/*******************************************************************************
DlistForEach() - iterates throu list and returns return value of do_func							
				 if return value is a non-zero, stops iterations

Time complexity: O(1).
*******************************************************************************/
int DlistForEach(							
		dlist_iter_t from,					
		dlist_iter_t to,					
		int (*do_func)(void *data, void *params),					
		void *params)
{
	assert(from != NULL);	
	assert(to != NULL);
	assert(do_func != NULL);

	/* foreach element while (from != to) operate the do_func. */	
	while (from != to)
	{
		int res = do_func(from->data, params);
		if (res != 0)
		{
			return (res);
		}

		from = from->next;
	}
	
	return (0);
}

/*******************************************************************************
DlistFind() - returns iterator to the found node; if not found, returns "to".

Time complexity: O(n)
*******************************************************************************/							
dlist_iter_t DlistFind(							
		dlist_iter_t from,					
		dlist_iter_t to,					
		const void *to_find,					
		void *params,					
		int (*is_match)(const void *node_data, const void *to_find, void *params))
{
	assert(from != NULL);	
	assert(to != NULL);
	assert(is_match != NULL);

	/* foreach element while (from != to) chack if is_match. */	
	while (from != to)
	{
		if (1 == is_match(from->data, to_find, params))
		{
			return (from);
		}
		
		from = from->next;
	}
	
	return (to);
}

/*******************************************************************************
DlistSplice()- cut and pase range: "from"(including) - "to"(excluding) rigth
			   before where.
			   returns iterator to the last splied node
Time complexity: O(n).
*******************************************************************************/
void DlistSplice(dlist_iter_t where, dlist_iter_t from, dlist_iter_t to)
{
	dlist_iter_t save = NULL;
	
	assert(where != NULL);
	assert(from != NULL);
	assert(to != NULL);

	if (from != to)
	{
		/* The node before "from" points to "to", and vice versa
		   The node before "where" points to "from", and vice versa
		   The node before "to" points to "where", and vice versa */
		save = from->prev;
		where->prev->next = from;
		from->prev->next = to;
		from->prev = where->prev;
		to->prev->next = where;
		where->prev = to->prev;	
		to->prev = save;
	}
}


