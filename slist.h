#ifndef SLIST_H_						
#define SLIST_H_						
						
#include <stddef.h> /* size_t */						
		
typedef struct slist_node slist_node_t;

struct slist_node	
{	
	void *data;
	slist_node_t *next;
};	
	
				
/* next parameter can be also NULL */						
slist_node_t *SListCreateAndInitNode(void *data, slist_node_t *next);	

void SListFreeAll(slist_node_t *head);						
						
/* Invalidate existing pointers to the where */						
/* Returns NULL when fails */						
slist_node_t *SListInsert(slist_node_t *insert_node, slist_node_t *where);						
						
/* Returns NULL when fails */						
slist_node_t *SListInsertAfter(slist_node_t *insert_node, slist_node_t *where);						
						
/* Returns a pointer to the removed node */						
/* Where must not be last node */						
slist_node_t *SListRemove(slist_node_t *where);						
						
/* Returns a pointer to the removed node */						
/* Where must not be last node */						
slist_node_t *SListRemoveAfter(slist_node_t *where);						
						
/* Returns NULL if didnt find anything */						
slist_node_t *SListFind(slist_node_t *head, const void* to_find, void *params,   int (*is_match)(const void *node_data, const void *to_find, void *params));						
						
int SListForEach(slist_node_t *head, int (*do_func)(void *params, void *data), void *params);
slist_node_t *SListFlip(slist_node_t *head);						
int SListHasLoop(const slist_node_t *head);						
size_t SListSize(const slist_node_t *head);						
						
/* Returns NULL if didnt find anything */						
slist_node_t *SListFindIntersection(slist_node_t *head1, slist_node_t *head2);						
		



/* */

#endif /* SLIST_H_ */

