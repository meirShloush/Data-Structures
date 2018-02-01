#include <stdlib.h> /* for malloc */
#include <assert.h> /* for assert */
#include <string.h> /* for memcpy */

#include "dyn_vec.h"

struct dyn_vec
{
	size_t item_size;
	size_t num_items;
	void *top; /* top of the vec */
	void *start; 
};

/****************************************************************
Resived item_size and num_items, and return pointer to dyn_vec,
if the allocation is fail return NULL
****************************************************************/
dyn_vec_t *DynVecCreate(size_t item_size, size_t num_items)
{
	void *tmp = NULL;
	
	dyn_vec_t *new_dyn_vec;
	
	assert(item_size);
	assert(num_items);
	
	/* memory allocation for struct */
	new_dyn_vec = (dyn_vec_t *)malloc (sizeof(dyn_vec_t));
	if(NULL == new_dyn_vec)
	{
		return (NULL);
	}
	
	/* memory allocation for dyn_vec */
	tmp = calloc(num_items, item_size);
	if(NULL == tmp)
	{
		free(new_dyn_vec);
		new_dyn_vec = NULL;

		return (NULL);
	}
	
	/* Initializing destarts of the dyn_vec */
	new_dyn_vec->top = (void *)((size_t)tmp + (num_items * item_size));
	new_dyn_vec->start = tmp;
	new_dyn_vec->item_size = item_size;
	new_dyn_vec->num_items = num_items;

	return (new_dyn_vec);
}

/****************************************************************
Destroy the Stack and free the memory
****************************************************************/
void DynVecDestroy(dyn_vec_t *vec)
{
	assert(vec != NULL);
	
	free(vec->start);
	vec->start = NULL;

	free(vec);
	vec = NULL;
}

/****************************************************************
Return the num of the item in the dyn_vec
****************************************************************/
size_t DynVecSize(const dyn_vec_t *vec)
{
	assert(vec != NULL);

	return(((size_t)vec->top - (size_t)vec->start) / vec->item_size);
}

/****************************************************************
Inserts an item at the top of the dyn_vec.
return 0 if sucsses, and 1 if fail.
****************************************************************/
int DynVecPushBack(dyn_vec_t *vec, const void *item)
{
	assert(vec != NULL);
	assert(item != NULL);
	
	/* if there are overflowe - realloc */
	if (DynVecSize(vec) == DynVecCapacity(vec))
	{
		int tmp = DynVecReserve(vec, vec->num_items * 2);
	
		if(tmp)
		{
			return(1);
		}
	}
	
	memcpy(vec->top, item, vec->item_size);
	vec->top = (void *)((size_t)vec->top + vec->item_size);

	return (0);
}	

/****************************************************************
Removes the item at the top of the dyn_vec.
****************************************************************/
void DynVecPopBack(dyn_vec_t *vec)
{
	assert(vec != NULL);
	
	vec->top = (void *)((size_t)vec->top - vec->item_size);
}

/****************************************************************
resived an index of dyn_vec, and return a pointer to 
****************************************************************/
void *DynVecGetItemAddress(const dyn_vec_t *vec, size_t index)
{
	assert(vec != NULL);
	assert(DynVecCapacity(vec) > index);
	
	return (void *)((size_t)vec->start + (index * vec->item_size));
}

/****************************************************************
return the capacity of dyn_vec 
****************************************************************/
size_t DynVecCapacity(const dyn_vec_t *vec)
{
	assert(vec != NULL);

	return (vec->num_items);
}

/****************************************************************
resived an dyn_vec and new_capacity, 
and increase the dyn_vec as a new_capacity size 
****************************************************************/
int DynVecReserve(dyn_vec_t *vec, size_t new_capacity)
{
	void *new_start = NULL;
	
	assert(vec->num_items < new_capacity);
	assert(vec != NULL);
		
	/* memory allocation for dyn_vec */	
	new_start = realloc(vec->start, new_capacity * vec->item_size);
	if(NULL == new_start)
	{
		return (1);
	}
	
	/* Initializing destarts of the dyn_vec */
	vec->top = (void *)((size_t)new_start + (DynVecSize(vec) * vec->item_size));
	vec->start = new_start;
	vec->num_items = new_capacity; 
	
	return (0);
}

