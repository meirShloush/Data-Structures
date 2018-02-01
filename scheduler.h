#ifndef SCHEDULER_H_							
#define SCHEDULER_H_							
							
#include <stddef.h> /* size_t */							
							
#include "uuid.h"
						

typedef struct scheduler scheduler_t;
							
scheduler_t *SchedulerCreate(void);							
void SchedulerDestroy(scheduler_t *scheduler);							
int SchedulerIsEmpty(const scheduler_t *scheduler);							
uuid_t SchedulerAdd(scheduler_t *scheduler,
					int(*func)(void *params),
					void *params,
					unsigned long interval_sec);
							
int SchedulerRemove(scheduler_t *scheduler, uuid_t uid);							
void SchedulerStop(scheduler_t *scheduler);							
							
/* Returns 0 on success or 1 on failure */							
int SchedulerRun(scheduler_t *scheduler);							
void SchedulerClear(scheduler_t *scheduler);							
							
#endif /* SCHEDULER_H_ */


