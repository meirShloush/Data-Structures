#include <stddef.h> /* for size_t */	
#include <stdlib.h> /* for malloc */
#include <string.h> /* for memcpy */
#include <assert.h> /* for assert */

#include "circ_buf.h"

struct circ_buf
{
	size_t capacity;
	size_t read_index;
	size_t size; /* write is read_index + size */
	char data[1];
};
	
enum circ_buf_errno circ_buf_e = 0;

/*****************************************************************************
craeation the buffer, and return pointer to it 
*****************************************************************************/
circ_buf_t *CircBufCreate(size_t capacity)
{	
	circ_buf_t *new_circ_buf = NULL;
	
	if (0 == capacity)
	{
		capacity = 1;
	}
	
	/* allocation memmory */
	new_circ_buf = (circ_buf_t *)malloc(sizeof(circ_buf_t) + capacity);
	if (NULL == new_circ_buf)
	{
		return (NULL);
	}
	
	/* initialize the management struct fields */
	new_circ_buf->capacity = capacity;  
	new_circ_buf->read_index = 0;  
	new_circ_buf->size = 0;  

	return (new_circ_buf);
}

/*****************************************************************************
 free the allocated 
*****************************************************************************/
void CircBufDestroy(circ_buf_t *buf)
{
	free(buf); buf = NULL;
}

/*****************************************************************************
 return how much free space left.
*****************************************************************************/
size_t CircBufFreeSpace(const circ_buf_t *buf)
{
	assert(buf !=NULL);
	
	return (buf->capacity - buf->size);
}

/*****************************************************************************
 return thr capscity of buf.
*****************************************************************************/
size_t CircBufCapacity(const circ_buf_t *buf)
{
	assert(buf !=NULL);

	return (buf->capacity);
}

/*****************************************************************************
 return 1 - if buf is empty, and 0 - if not.
*****************************************************************************/
int CircBufIsEmpty(const circ_buf_t *buf)
{
	assert(buf !=NULL);

	return (!(buf->size));
}
	
/*****************************************************************************
 copy data from buffer to extern_buf,
 in case that count > data size - update the errno = CIRC_BUF_UNDERFLOW  
 return how much chars is read
*****************************************************************************/
size_t CircBufRead(circ_buf_t *buf, void *extern_buf, size_t count)
{
	/* The number of chars that should be read from read_index to index of (capacity - 1) */
	size_t copy_from_end_buf = 0; 

	assert(buf !=NULL);
	assert(extern_buf !=NULL);
	
	circ_buf_e = CIRC_BUF_SUCCESS;		
	
	/* underflow case */
	if (count > buf->size)
	{
		count = buf->size;
		circ_buf_e = CIRC_BUF_UNDERFLOW;		
	}
	
	copy_from_end_buf = ((buf->capacity - buf->read_index) < count) ? (buf->capacity - buf->read_index) : (count);

	/* Copy to extern_buf, from buf, start in read_index */
	memcpy (extern_buf, (buf->data + buf->read_index), copy_from_end_buf);
	
	/* Copy from the beginning of buf if necessery */
	memcpy (((char *)extern_buf + copy_from_end_buf), (buf->data), (count - copy_from_end_buf));

	/* update the management struct fields */
	buf->read_index = (buf->read_index + count) % buf->capacity;
	buf->size -=count;
	
	return (count);
}	
	
/*****************************************************************************
 copy data from extern_buf to buffer,
 in case that count > free space - update the errno = CIRC_BUF_OVERFLOW
 return how much chars is writen
*****************************************************************************/
size_t CircBufWrite (circ_buf_t *buf, void *extern_buf, size_t count)
{	
	size_t copy_at_end_buf = 0; /* The number of chars that should be write from write_index to index of (capacity - 1) */
	size_t write_index = (buf->read_index + buf->size) % buf->capacity;;
	
	assert(buf !=NULL);
	assert(extern_buf !=NULL);

	circ_buf_e = CIRC_BUF_SUCCESS;		
	
	/* overflow case */
	if (count > CircBufFreeSpace(buf))
	{
		count = CircBufFreeSpace(buf);
		circ_buf_e = CIRC_BUF_OVERFLOW;		
	}
	
	copy_at_end_buf = ((buf->capacity - write_index) < count) ? (buf->capacity - write_index) : (count);
	
	/* write to buf, start in write_index */
	memcpy((buf->data + write_index), extern_buf, copy_at_end_buf);
	
	/* write to the beginning of buf if necessery */
	memcpy((buf->data), ((char *)extern_buf + copy_at_end_buf), (count - copy_at_end_buf));
	
	/* update the management struct field */
	buf->size += count;
	
	return (count);	
}

