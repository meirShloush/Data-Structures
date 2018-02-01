#include <string.h> /* for memcpy */
#include <stdlib.h> /* for malloc */
#include <assert.h> /* for assert */
#include "stack.h"  

struct stack
{
	void *top;
	size_t size_of_element;
	size_t max_of_elements;
	char start[1];
};


/****************************************************************
Resived element_size and max_elements, and return pointer to Stack,
if the allocation is fail return NULL
****************************************************************/
stack_t *StackCreate(size_t element_size, size_t max_elements)
{
	stack_t *new_stack;
	
	assert(0 != element_size);
	assert(0 != max_elements);
		
	/* memory allocation */
	new_stack = (stack_t *)malloc(sizeof (*new_stack) + 
								(element_size * max_elements));
	if(new_stack == NULL)
	{
		return (NULL);
	}
	
	/* Initializing details of the Stack */
	new_stack->top = (void *)new_stack->start;
	new_stack->size_of_element = element_size;
	new_stack->max_of_elements = max_elements;

	return (new_stack);
}


/****************************************************************
Destroy the Stack and free the memory
****************************************************************/
void StackDestroy(stack_t *stack)
{
	assert(NULL != stack);
	
	free(stack);
	stack = NULL;
}


/****************************************************************
Inserts an object at the top of the Stack.
return 0 if sucsses, and -1 in overflowe case.
****************************************************************/
int StackPush(stack_t *stack, const void *element)
{
	void *tmp = NULL;

	assert(NULL != element);
	assert(NULL != stack);
		
	/* check overflowe */
	if (StackSize(stack) == stack->max_of_elements)
	{
		return (-1);
	}
	
	tmp = memcpy(stack->top, element, stack->size_of_element);
	if(tmp == NULL)
	{
		return (-1);
	}
	
	stack->top = (void *)((size_t)stack->top + stack->size_of_element);

	return (0);
}


/****************************************************************
Removes the object at the top of the Stack.
return 0 if sucsses, and -1 in underflowe case.
****************************************************************/
void StackPop(stack_t *stack)
{
	assert(NULL != stack);

	stack->top = (void *)((size_t)stack->top - stack->size_of_element);
}


/****************************************************************
Returns the object at the top of the Stack without removing it.
if the stack is empty return NULL.
****************************************************************/
void *StackPeek(const stack_t *stack)
{
	assert(NULL != stack);
	
	return ((void *)((size_t)stack->top - stack->size_of_element ));
}


/****************************************************************
Return the num of the element in the Stack
****************************************************************/
size_t StackSize(const stack_t *stack)
{
	assert(NULL != stack);

	return (((size_t)stack->top - (size_t)stack->start) / stack->size_of_element);
}

