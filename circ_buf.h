#ifndef CIRC_BUF_H_            
#define CIRC_BUF_H_

typedef struct circ_buf circ_buf_t;


enum circ_buf_errno
{					
	CIRC_BUF_SUCCESS = 0,	
	CIRC_BUF_OVERFLOW = 1,
	CIRC_BUF_UNDERFLOW = 2	
};	

extern enum circ_buf_errno circ_buf_e; 


circ_buf_t *CircBufCreate(size_t capacity);					
void CircBufDestroy(circ_buf_t *buf);					
size_t CircBufFreeSpace(const circ_buf_t *buf);					
size_t CircBufCapacity(const circ_buf_t *buf);					
int CircBufIsEmpty(const circ_buf_t *buf);					
		
/* return how much chars is read */
size_t CircBufRead(circ_buf_t *buf, void *extern_buf, size_t count);					
					
/* return how much chars is writen */					
size_t CircBufWrite(circ_buf_t *buf, void *extern_buf, size_t count);		


#endif /* CIRC_BUF_H_ */    
