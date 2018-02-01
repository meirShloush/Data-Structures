#include <assert.h> 	/* for assert */							
#include <stddef.h> 	/* for size_t */							
#include <stdlib.h>		/* for malloc*/		
#include <unistd.h> 	/* for sleep */	

#include "scheduler.h"
#include "scheduler_task.h"
#include "pq.h"


struct scheduler
{							
	pq_t	*tasks;
	task_t	*current_task;
	int 	is_running;		/* flag, hold 1 if scheduler is running */
	int 	is_remove;		/* flag, hold 1 if task remove itself */
};

/*******************************************************************************
returns pointer to new scheduler, or NULL on faliure
*******************************************************************************/				
scheduler_t *SchedulerCreate(void)
{
	/* Allocate memory for scheduler struct */
	scheduler_t *new_scheduler = (scheduler_t *)malloc(sizeof(*new_scheduler));
	if (NULL == new_scheduler)
	{
		return (NULL);
	}
	
	/* Allocate memory for pq of tasks */
	new_scheduler->tasks = PQCreate(NULL, SchedulerTaskIsBefore);
	if (NULL == new_scheduler->tasks)
	{
		free(new_scheduler); new_scheduler = NULL;
		return (NULL);
	}
	
	/* assignment struct's fields */
	new_scheduler->current_task = NULL;
	new_scheduler->is_running = 0;
	new_scheduler->is_remove = 0;
	
	return (new_scheduler);
}

/*******************************************************************************
Destroys a scheduler

Time complexity: O(n).
*******************************************************************************/
void SchedulerDestroy(scheduler_t *scheduler)
{
	assert(scheduler != NULL);

	/* free all tasks */
	SchedulerClear(scheduler);
	
	/* free pq */	
	PQDestroy(scheduler->tasks);

	/* free scheduler */		
	free(scheduler); scheduler = NULL;
}

/*******************************************************************************
Returns 1 if the scheduler is empty, or 0 if not.

there is no assertion that (scheduler->tasks != NULL) &&
					 	   (NULL == scheduler->current_task),
Because during running time - scheduler != null

Time complexity: O(1).
*******************************************************************************/
int SchedulerIsEmpty(const scheduler_t *scheduler)	
{
	assert(scheduler != NULL);

	/* check the current task too */
	return ((PQIsempty(scheduler->tasks)) && (NULL == scheduler->current_task));
}

/*******************************************************************************
Inserts a new element according to its priority into the scheduler.

Time complexity: O(n).
*******************************************************************************/
uuid_t SchedulerAdd(scheduler_t *scheduler,
					int(*func)(void *params),
					void *params, 
					unsigned long interval_sec)
{
	task_t *new_task = NULL;
	
	assert(scheduler != NULL);
	assert(func != NULL);
	
	new_task = SchedulerTaskCreate(func, params, interval_sec);
	if (NULL == new_task)
	{
		return (UuidGetInvalidID());
	}
	
	if (PQEnqueue(scheduler->tasks, new_task) != 0)
	{
		free(new_task); new_task = NULL;

		return (UuidGetInvalidID());
	}
	
	return (SchedulerTaskGetId(new_task));
}

/*******************************************************************************
Removes a specific task according the uid, and returns 1.
If didn't find returns 0.

Time complexity: O(n).
*******************************************************************************/
int SchedulerRemove(scheduler_t *scheduler, uuid_t uid)
{
	void *to_remove = NULL;

	assert(scheduler != NULL);

	/* if current_task try to remove itself */
	if ((scheduler->current_task != NULL) && 
		(SchedulerTaskIsMatch(scheduler->current_task, &uid, NULL)))
	{
		scheduler->is_remove = 1;
		return (0);
	}

	to_remove = PQRemove(scheduler->tasks, &uid, NULL, SchedulerTaskIsMatch);
	if (NULL == to_remove)
	{
		return (1);
	}
	
	SchedulerTaskDestroy(to_remove);
	
	return (0);
}

/*******************************************************************************
Stop the scheduler runing.

Time complexity: O(1).
*******************************************************************************/
void SchedulerStop(scheduler_t *scheduler)
{
	assert(scheduler != NULL);

	scheduler->is_running = 0;
}

/*******************************************************************************
Returns 0 on success, or 1 on failure.

Time complexity: O(?)
*******************************************************************************/
int SchedulerRun(scheduler_t *scheduler)
{
	assert(scheduler != NULL);

	if (1 == scheduler->is_running)
	{
		return (1);
	}
	
	scheduler->is_running = 1;	

	/* run tasks until stop function or until scheduler is empty */
	while ((SchedulerIsEmpty(scheduler) != 1) && (1 == scheduler->is_running))
	{	
		scheduler->current_task = (task_t *)PQDequeue(scheduler->tasks);
		
		/* if rturen from SchedulerTaskRun is not error, 
		and the task is not trying to removes itself */
		if (0 == (SchedulerTaskRun(scheduler->current_task)) && 
			(0 == scheduler->is_remove))
		{		
			SchedulerTaskUpdate(scheduler->current_task);

			if (1 == PQEnqueue(scheduler->tasks, scheduler->current_task))
			{
				SchedulerTaskDestroy(scheduler->current_task);
				return (1);
			}
		}
		else
		{
			SchedulerTaskDestroy(scheduler->current_task);
		}
		
		scheduler->current_task = NULL;
	}
	
	scheduler->is_running = 0;
	scheduler->is_remove = 0;

	return (0);
}

/*******************************************************************************
clear the scheduler tasks

Time complexity: O(n).
*******************************************************************************/
void SchedulerClear(scheduler_t *scheduler)
{	
	assert(scheduler != NULL);

	while ((PQIsempty(scheduler->tasks)) != 1)
	{
		task_t *task = (task_t*)PQDequeue(scheduler->tasks);

		SchedulerTaskDestroy(task);		
	}
}

					
