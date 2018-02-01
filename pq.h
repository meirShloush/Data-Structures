#include <stddef.h> /* size_t */														
#include "srt_list.h" /* size_t */						
														
typedef struct pq pq_t;	
								
pq_t *PQCreate(void *params,											
			int (*is_before)(const void *data1,								
			const void *data2,				
			void *params));
			
			
void PQDestroy(pq_t *pq);

size_t PQSize(const pq_t *pq);
									
int PQIsempty(const pq_t *pq);	
									
int PQEnqueue(pq_t *pq, void *data);				

void *PQDequeue(pq_t *pq);

void *PQPeek(pq_t *pq);	
									
void PQClear(pq_t *pq);
						
void *PQRemove(pq_t *pq, const void *to_find, void *params,											
				int (*is_match)(const void *data,									
				const void *to_find,					
				void *params));
				

	

