#include <assert.h> /* for assert */
#include <stddef.h> /* for size_t */

#include "fsm.h"

#define WORD_SIZE sizeof(size_t)

struct fsm
{					
	size_t num_blocks;				
	size_t block_size;				
	size_t offset;		/* holds offset in bytes to the next free block */			
};
	

/*******************************************************************************					
return block size include header
*******************************************************************************/
size_t Alignment(size_t block_size)
{
	if ((block_size % WORD_SIZE) != 0)
	{ 			
		block_size /= WORD_SIZE;

		++block_size;

		block_size *= WORD_SIZE;
	}

	/* for header */
	block_size += WORD_SIZE;

	return (block_size);
}

/*******************************************************************************					
returns size in bytes of memory necessary for pool -					
according to num_blocks and block_size 
*******************************************************************************/			
size_t FsmSuggestSize(size_t num_blocks, size_t block_size)
{
	block_size = Alignment(block_size);
	
	return (sizeof(fsm_t) + (num_blocks * block_size));
}
					
/*******************************************************************************
initialize the memory buffer 			
*******************************************************************************/
fsm_t *FsmInit(void *mem, size_t num_blocks, size_t block_size)
{	
	int i = 0;
	char *header = NULL;
	fsm_t *new_fsm = (fsm_t *)mem;

	assert(mem != NULL);
	assert(num_blocks != 0);
	assert(block_size != 0);

	block_size = Alignment(block_size);
	
	new_fsm->block_size = block_size;
	new_fsm->num_blocks = num_blocks;
	new_fsm->offset = sizeof(fsm_t);
	
	header = (char *)mem + sizeof(fsm_t);
	
	/* assign to headers the offset from the start to the next free block */
	for (i = 1; i < num_blocks; ++i)
	{	
		*(size_t *)header = sizeof(fsm_t) + (i * block_size);
		
		header += block_size;
	}
	
	/* Assign 0 to the last header */
	*(size_t *)header = 0;
	
	return (new_fsm);
}

/*******************************************************************************
allocate a single block 
*******************************************************************************/
void *FsmAlloc(fsm_t *fsm)
{
	void *ret = NULL;
	char *header = NULL;
	size_t save = 0;
	
	assert(fsm != NULL);
	
	/* check if there is enough space to allocate */
	if (0 == fsm->offset)
	{
		return (NULL);		
	}
	
	header = (char *)fsm + fsm->offset;

	ret = (char *)fsm + fsm->offset + WORD_SIZE;
	
	/* swap */
	save = *(size_t *)header;
	*(size_t *)header = fsm->offset;
	fsm->offset = save; 

	return (ret);		
}

/*******************************************************************************
free an allocated block O(1)
*******************************************************************************/
void FsmFree(void *block)
{	
	if (block != NULL)
	{
		fsm_t *fsm = NULL;
		size_t save = 0;

		/* change block to be pointing to header */
		block = (char *)block - WORD_SIZE;

		fsm = (fsm_t *)((char *)block - *(size_t *)block);

		/* swap */
		save = fsm->offset;
		fsm->offset = *(size_t *)block ;
		*((size_t *)block) = save;
	}
}

/*******************************************************************************
 returns the number of free blocks O(n) 
*******************************************************************************/
size_t FsmCountFree(const fsm_t *fsm)
{
	size_t counter = 0;
	size_t i = 0;
	size_t offset = 0;
	
	assert(fsm != NULL);
	
	offset = sizeof(fsm_t);
	
	for (i = 0; i < fsm->num_blocks; ++i)
	{
		/* if number in header not equal to his offset */
		if (*((char *)fsm + offset) != offset)
		{
			++counter;
		}

		offset += fsm->block_size;
	}
	
	return (counter);
}

