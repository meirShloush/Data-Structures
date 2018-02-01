#ifndef SRT_LIST_H_									
#define SRT_LIST_H_									
									
#include <stddef.h> /* for size_t */									

#include "dlist.h"


typedef struct srt_list srt_list_t;	

typedef struct srt_list_iter									
{									
	struct srt_list_iter_info *info;								
} srt_list_iter_t;									
															
srt_list_t *SrtListCreate(void *params,									
						int (*is_before)(const void *data1,	const void *data2, void *params));		
													
void SrtListDestroy(srt_list_t *srt_list);									
									
size_t SrtListSize(const srt_list_t *srt_list);									
									
int SrtListIsEmpty(const srt_list_t *srt_list);									
									
/* Return iter to inserted node on success, end on failure */									
srt_list_iter_t SrtListInsert(srt_list_t *srt_list, void *data);									
									
/* Return iter to the next node */									
srt_list_iter_t SrtListRemove(srt_list_iter_t whom);									
									
srt_list_iter_t SrtListBegin(const srt_list_t *srt_list);									
									
srt_list_iter_t SrtListEnd(const srt_list_t *srt_list);									
									
srt_list_iter_t SrtListPrev(srt_list_iter_t current);									
									
srt_list_iter_t SrtListNext(srt_list_iter_t current);									
									
void *SrtListGetData(srt_list_iter_t iter);									
									
int SrtListIsSameIter(srt_list_iter_t iter1, srt_list_iter_t iter2);									
													
/* iterate through the sorted list.									
returns the value of do_func: if the value is a non-zero, stops iterations */									
int SrtListForEach(srt_list_iter_t from,									
				srt_list_iter_t to,					
				void *params,					
				int (*do_func)(void *data, void *params));					
									
									
srt_list_iter_t SrtListFind(srt_list_t *srt_list,									
							srt_list_iter_t from,		
							srt_list_iter_t to,		
							const void *to_find);		
									
srt_list_iter_t SrtListFindIf(srt_list_iter_t from,											
							srt_list_iter_t to,				
							const void *to_find,				
							void *params,				
						int (*is_match)(const void *node_data,					
										const void *to_find,	
										void *params));		
																		
void *SrtListPopBack(srt_list_t *srt_list);
									
void *SrtListPopFront(srt_list_t *srt_list);									
									
/* src becomes empty */									
void SrtListMerge(srt_list_t *dest, srt_list_t *src);
								

#endif /* SRT_LIST_H_ */

