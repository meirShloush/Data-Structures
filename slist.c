#include <assert.h> /* for assert */
#include <stddef.h> /* for size_t */
#include <stdlib.h> /* for malloc */

#include "slist.h"


/*******************************************************************************
Creates a new node and initializes it.
Both data and next are allowed to be NULL.
Time complexity: O(1).
*******************************************************************************/
slist_node_t *SListCreateAndInitNode(void *data, slist_node_t *next)
{
	slist_node_t *node = (slist_node_t *)malloc(sizeof(slist_node_t));
	if (NULL == node)
	{
		return (NULL);
	}
	
	node->data = data;
	node->next = next;

	return (node);
}

/*******************************************************************************
frees all nodes in a linked list that starts at the given head.
Time complexity: O(n).
*******************************************************************************/
void SListFreeAll(slist_node_t *head)
{
	slist_node_t *next_node = head;
	
	assert(SListHasLoop(head) != 1);

	while (next_node != NULL)
	{
		head = next_node;
		next_node = next_node->next;
		free(head); head = NULL;
	}
}						

/*******************************************************************************
Helper function for Insert and Remove.	
Time complexity: O(1)
*******************************************************************************/
void Swap(void **p1, void **p2)
{
	void *tmp = *p1;
	*p1 = *p2;
	*p2 = tmp;
}

/*******************************************************************************
inserts node before node pointed to by where.
returns new pointer to insert_node.
Invalidate existing pointers to the where					
Time complexity: O(1).
*******************************************************************************/
slist_node_t *SListInsert(slist_node_t *insert_node, slist_node_t *where)
{	
	assert(insert_node != NULL);
	assert(where != NULL);
	
	Swap(&where->data, &insert_node->data);
	
	insert_node->next = where->next;
	where->next = insert_node;
	
	return (where);
}

/*******************************************************************************
Removes the given node, and returns a pointer to that node
Time complexity: O(1).
*******************************************************************************/
slist_node_t *SListInsertAfter(slist_node_t *insert_node, slist_node_t *where)
{
	assert(insert_node != NULL);
	assert(where != NULL);

	/* Assign where->next to res and make where pointing to the next node */
	insert_node->next = where->next;
	where->next = insert_node;
	
	return (insert_node);
}
						
/*******************************************************************************
Removes the given node, and Returns a pointer to her, 					
Where must not be last node 
Time complexity: O(1).
*******************************************************************************/
slist_node_t *SListRemove(slist_node_t *where)
{
	slist_node_t *res = NULL;

	assert(where != NULL);
	assert(where->next != NULL);
	
	res = where->next;
	
	Swap(&where->data, &where->next->data);
	where->next = where->next->next;
	/*res->next = NULL;	
	*/
	return (res);
}

/*******************************************************************************
Removes the given node, and Returns a pointer to her, 					
Where must not be last node
Time complexity: O(1).
*******************************************************************************/
slist_node_t *SListRemoveAfter(slist_node_t *where)
{
	slist_node_t *res = NULL;
	
	assert(where != NULL);
	assert(where->next != NULL);
	
	res = where->next;
	where->next = where->next->next;
	/*res->next = NULL;
	*/
	return (res);
}						
	
/*******************************************************************************
Returns the node in which the requested data found
Returns NULL if didnt find anything
Time complexity: O(n).
*******************************************************************************/
slist_node_t *SListFind(slist_node_t *head, const void* to_find, void *params,
						int(*is_match)
						(const void *node_data,
						 const void *to_find,
						 void *params))
{
	assert(is_match != NULL);
	assert(SListHasLoop(head) != 1);

	while (head != NULL)
	{	
		if (1 == is_match(head->data, to_find, params))
		{
			return (head);
		}
		head = head->next;
	}
	
	return (NULL);
}

/*******************************************************************************
-Operates the func on every element in slist.
when fails returns do_func's failure value and stops iterating
Time complexity: O(n).
*******************************************************************************/
int SListForEach(slist_node_t *head,
				 int (*do_func)(void *params, void *data),
				 void *params)
{
	int res = 0;
	
	assert(SListHasLoop(head) != 1);
	assert(do_func != NULL);

	/* foreach element in list operate the function. */	
	while ((head != NULL) && (0 == res))
	{	
		res = do_func(params, head->data);	
		head = head->next;
	}
	
	return (res);
}

/*******************************************************************************
flip the slist, and return the new head
Time complexity: O(n).
*******************************************************************************/
slist_node_t *SListFlip(slist_node_t *head)
{	
	slist_node_t *prev = NULL;
	slist_node_t *nex = NULL;

	assert(head != NULL);
	assert(SListHasLoop(head) != 1);

	nex = head->next;
	
	while (nex != NULL)
	{
		/* every element will point to the previous one */		
		head->next = prev;
		
		prev = head;
		head = nex;
		nex = nex->next;
	}
	
	/* the tail will be the new head, and point to prev */
	head->next = prev;

	return (head);
}

/*******************************************************************************
Returns 1 if slist has a loop and returns 0 otherwise
Time complexity: O(n).
*******************************************************************************/
int SListHasLoop(const slist_node_t *head)
{
	const slist_node_t *one_step = head;  /* Jumps 2 steps each time */
	const slist_node_t *two_steps = head; /* Jumps 1 step each time */
	
	if(NULL == head)
	{
			return (0);
	}

	while ((two_steps != NULL) && (two_steps->next != NULL))
	{
		one_step = one_step->next;
		two_steps = two_steps->next->next;
		
		if (one_step == two_steps)
		{
			return (1);
		}
	}
	
	return (0);
}

/*******************************************************************************
return the num of elements held in SList
Time complexity: O(n).
*******************************************************************************/
size_t SListSize(const slist_node_t *head)
{
	size_t counter = 0;
	
	assert(SListHasLoop(head) != 1);
		
	while (head != NULL)
	{	
		head = head->next;
		++counter;
	}
	
	return (counter);
}

/*******************************************************************************
Returns the first node that exists in both slist1 and slist2
Returns NULL if didnt find anything 
Time complexity: O(n).
*******************************************************************************/
slist_node_t *SListFindIntersection(slist_node_t *head1, slist_node_t *head2)
{
	size_t size1 = SListSize(head1);
	size_t size2 = SListSize(head2);

	/* promotes the pointer of the longer slist (head1 or head2),
	until its offset from the end is equal to the shorter's size */
	while (size1 > size2)
	{
		head1 = head1->next;
		--size1;
	}

	while (size2 > size1)
	{
		head2 = head2->next;
		--size2;
	}

	/* Checks for equality between the nodes */
	while (head1 != NULL)
	{
		if (head1 == head2)
		{
			return (head1);
		}
		
		head1 = head1->next;
		head2 = head2->next;
	}
	
	return (NULL);
}
						
