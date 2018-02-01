#ifndef STACK_H_
#define STACK_H_
#include <stddef.h> /* size_t */

typedef struct stack stack_t;

stack_t *StackCreate(size_t element_size, size_t max_elements);		
void StackDestroy(stack_t *stack);		
int StackPush(stack_t *stack, const void *element);		
void StackPop(stack_t *stack);		
void *StackPeek(const stack_t *stack);		
size_t StackSize(const stack_t *stack);

#endif /* STACK_H_ */
