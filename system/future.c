#include <xinu.h>
#include <stdio.h>
#include <future.h>


future_t* future_alloc(future_mode_t mode)
{
	future_t *future;
	int queue_value;

	future = (future_t*)getmem(sizeof(future_t));
	
	if(future == NULL)
	{
		printf("getmem error\n");
		return NULL;
	}

	future->state = FUTURE_EMPTY;
	future->mode = mode;
	if((queue_value = newqueue()) == SYSERR)
	{
		printf("get_queue allocation error\n");
		return NULL;
	}
	future->get_queue = queue_value;

	if((queue_value = newqueue()) == SYSERR)
	{
		printf("set_queue allocation error\n");
		return NULL;
	}
	future->set_queue = queue_value;

	return future;
}

int future_queue_set(future_t* future, int *value)
{
	int dequeue_value;

	if((dequeue_value=dequeue(future->get_queue))!=EMPTY)
	{
		future->value = *value;
		future->state = FUTURE_READY;
		//printf("Resuming %d\n", dequeue_value);
		resume(dequeue_value);
	}
	else
	{
		enqueue(getpid(), future->set_queue);
		suspend(getpid());
		future->value = *value;
		future->state = FUTURE_READY;
	}
	return OK;
}

syscall future_set(future_t* future, int* value)
{
	int dequeue_pid;
	if(future->mode == FUTURE_QUEUE)
	{
		return future_queue_set(future, value);
	}

	if((future->mode == FUTURE_EXCLUSIVE || future->mode ==  FUTURE_SHARED) && (future->state == FUTURE_READY))
	{
		printf("ERROR: Future is already set\n");
		return SYSERR;
	}
	else
	{
		future->value = *value;
	}
	
	if(future->state == FUTURE_WAITING){
		dequeue_pid = dequeue(future->get_queue);

		if(dequeue_pid==SYSERR)
		{
			printf("Dequeue SYSERR in future_set\n");
			return SYSERR;
		}
		
		future->state = FUTURE_READY;
		while(dequeue_pid!=EMPTY)
		{
			//printf("%d resuming %d\n",getpid(), dequeue_pid);
			resume(dequeue_pid);
			dequeue_pid = dequeue(future->get_queue);			
		}
	}
	future->state = FUTURE_READY;
	return OK;
}


int future_queue_get(future_t* future, int *value)
{
	int dequeue_value;
	future->state = FUTURE_WAITING;

	if((dequeue_value=dequeue(future->set_queue))==EMPTY)
	{
		enqueue(getpid(), future->get_queue);
		//printf("Suspending %d\n",getpid());
		suspend(getpid());
	}
	else
	{
		resume(dequeue_value);
	}
	// while(future->state != FUTURE_READY)
	// 	;
	*value = future->value;
	future->state = FUTURE_EMPTY;
	return OK;
}

syscall future_get(future_t* future, int *value)
{
	int ret_val;
	if(future->mode == FUTURE_QUEUE)
	{
		return future_queue_get(future, value);
	}

	if(future->mode == FUTURE_EXCLUSIVE && future->state == FUTURE_WAITING)
	{
		printf("ERROR: Only one process can get value in FUTURE_EXCLUSIVE mode\n");
		return SYSERR;
	}

	if(future->state == FUTURE_READY)
	{
		*value = future->value;
	}
	else
	{
		if(SYSERR == enqueue(getpid(), future->get_queue))
		{
			printf("Enqueue error in future_get\n");
			return SYSERR;
		}

		future->state = FUTURE_WAITING;
		//printf("suspending myself %d\n", getpid());
		suspend(getpid());
		*value = future->value;
	}
	if(future->mode!=FUTURE_SHARED)
	{
		future->state = FUTURE_EMPTY;
	}

	return OK;
}

syscall future_free(future_t* future)
{
	while(dequeue(future->get_queue)!=EMPTY);
	while(dequeue(future->set_queue)!=EMPTY);
	freemem(future, sizeof(future_t));
	return OK;
}