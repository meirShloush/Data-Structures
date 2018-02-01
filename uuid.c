#include <assert.h>		/* for assert */
#include <sys/time.h>	/* struct timeval */			
#include <sys/types.h>	/* pid_t */			
#include <stddef.h>		/* size_t */			
#include <unistd.h>

#include "uuid.h"
	
/*******************************************************************************
UuidCreate() - Creates a new uid, and return it
*******************************************************************************/					
uuid_t UuidCreate(void)
{
	static size_t counter;

	uuid_t res = {0};
	
	res.pid = getpid();			
	gettimeofday(&res.time, NULL);	
	res.ctr = counter;			

	++counter;
	
return (res);
}				

/*******************************************************************************
UuidIsequal() - return 1 if uid1 equal to uid2

Time complexity: O(1).
*******************************************************************************/				
int UuidIsequal(uuid_t uid1, uuid_t uid2)
{	
	return (((uid1.pid == uid2.pid) &&
			(uid1.time.tv_sec == uid2.time.tv_sec) &&
			(uid1.time.tv_usec == uid2.time.tv_usec) &&
			(uid1.ctr == uid2.ctr)) ? 1 : 0 );
}
		
/*******************************************************************************
UuidGetInvalidID() - return NULL uuid_t.

Time complexity: O(1).
*******************************************************************************/				
uuid_t UuidGetInvalidID(void)
{
	uuid_t res = {0};
	
	return (res);	
}



