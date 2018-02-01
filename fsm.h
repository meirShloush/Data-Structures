#ifndef FSM_H_
#define FSM_H_

typedef struct fsm fsm_t;
	
/* returns size in bytes of memory necessary for pool -
according to num_blocks and block_size */
size_t FsmSuggestSize(size_t num_blocks, size_t block_size);

/* initialize the memory buffer */
fsm_t *FsmInit(void *mem, size_t num_blocks, size_t block_size);

/* allocate a single block O(1) */
void *FsmAlloc(fsm_t *fsm);
					
/* "free" an allocated block  O(1) */
void FsmFree(void *block);
					
/* returns the number of free blocks */
size_t FsmCountFree(const fsm_t *fsm);
					
#endif /* FSM_H_ */

