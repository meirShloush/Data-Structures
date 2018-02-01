#ifndef DLIST_H_
#define DLIST_H_

#include <stddef.h> /* size_t */							
				
typedef struct dlist dlist_t;							
typedef struct dlist_node *dlist_iter_t;							
							
dlist_t *DlistCreate(void);							
							
void DlistDestroy(dlist_t *dlist);							
							
size_t DlistSize(const dlist_t *dlist);							
							
/* returns 1 if empty, 0 if not */							
int DlistIsEmpty(const dlist_t *dlist);							
							
dlist_iter_t DlistBegin(dlist_t *dlist);							
							
dlist_iter_t DlistEnd(dlist_t *dlist);							
							
dlist_iter_t DlistNext(dlist_iter_t current);							
							
dlist_iter_t DlistPrev(dlist_iter_t current);							
							
/* returns 1 if same, 0 if not */							
int DlistIsSameIter(dlist_iter_t iter1, dlist_iter_t iter2);							
							
void *DlistGetData(dlist_iter_t iter);							
							
/* returns iterator to inserted node on sucess							
/  or end iterator on faliure */							
dlist_iter_t DlistInsert(dlist_t *dlist, dlist_iter_t where, void *data);							
							
/* returns iterator to next node it the list */							
dlist_iter_t DlistErase(dlist_iter_t whom);							
							
/* returns iterator to the new node */							
dlist_iter_t DlistPushBack(dlist_t *dlist, void *data);							
							
/* returns iterator to the new node */							
dlist_iter_t DlistPushFront(dlist_t *dlist, void *data);							
							
/* returns data of the poped out node */							
void *DlistPopBack(dlist_t *dlist);							
							
/* returns data of the poped out node */							
void *DlistPopFront(dlist_t *dlist);							
							
/* iterates throu list and returns return value of do_func							
/  if return value is a non-zero, stops iterations */							
int DlistForEach(							
		dlist_iter_t from,					
		dlist_iter_t to,					
		int (*do_func)(void *data, void *params),					
		void *params);					
							
/* returns iterator to the found node							
/  if not found, returns "to" */							
dlist_iter_t DlistFind(							
		dlist_iter_t from,					
		dlist_iter_t to,					
		const void *to_find,					
		void *params,					
		int (*is_match)(const void *node_data, const void *to_find, void *params));					
							
/* cut and pase range: "from"(including) - "to"(excluding) rigt before where							
/  returns iterator to the last splied node */							
void DlistSplice(dlist_iter_t where, dlist_iter_t from, dlist_iter_t to);							
							
#endif /* DLIST_H_ */
