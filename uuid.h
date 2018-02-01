#ifndef UUID_H_            
#define UUID_H_


#include <sys/time.h>	/* struct timeval */			
#include <sys/types.h>	/* pid_t */			
#include <stddef.h>		/* size_t */			
				
typedef struct uuid				
{				
	pid_t pid;			
	struct timeval time;			
	size_t ctr;			
} uuid_t;				
						
uuid_t UuidCreate(void);

int UuidIsequal(uuid_t uid1, uuid_t uid2);				
				
uuid_t UuidGetInvalidID(void);

#endif /* UUID_H_ */    
