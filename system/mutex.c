#include <xinu.h>
#include <babybird.h>

volatile int test_lock = 0, test_unlock = 0;
volatile int cond_lock = 0, cond_unlock = 0;
//volatile int wait_process_pid;
int test_and_set(volatile int*);



syscall mutex_create(mutex_t *lock){
	lock->value = 1;
	lock->test_and_set_lock = 0;
	lock->q.head = 0;
	lock->q.tail = 0;
	return 0;
}
/**
* Decrement value
* if <0
**/
syscall mutex_lock(mutex_t *lock){	
	while(test_and_set(&lock->test_and_set_lock)==1)
		;
		
	//kprintf("Entered Critical section\t lock value %d\t address %x",lock->value, lock);
	//Critical section begins
	lock->value--;
	if(lock->value<0)
	{
		q_enqueue(getpid(),&lock->q);
		lock->test_and_set_lock = 0;
		suspend(getpid());
		
	}

	lock->test_and_set_lock = 0;
	return 0;
}

syscall mutex_unlock(mutex_t *lock)
{
	while(test_and_set(&lock->test_and_set_lock)==1)  
		;

	lock->value++;

	if(lock->value<=0)
	{
		resume(q_dequeue(&lock->q));
	}
	//kprintf("UNLOCKED\t lock value %d\n",lock->value);
	//lock->test_and_set_lock = 0;
	lock->test_and_set_lock = 0;
	return 0;
}

syscall cond_init(cond_t *cv)
{
	cv->value = -1;
	cv->test_and_set_lock = 0;
	cv->q.head = 0;
	cv->q.tail = 0;

	return 0;
}

syscall cond_wait(cond_t *cv, mutex_t *lock)
{
	//mutex_unlock(lock);
	while(test_and_set(&cv->test_and_set_lock)==1)  
		;
		q_enqueue(getpid(),&cv->q);
		mutex_unlock(lock);
		cv->test_and_set_lock = 0;
		suspend(getpid());
		mutex_lock(lock);
		
	
	return 0;
}

syscall cond_signal(cond_t *cv)
{
//	cv->value=1;

	//while(cv->value == -1);
	//resume(cv->value);

	while(test_and_set(&cv->test_and_set_lock)==1)  
		;

	while(cv->q.head != cv->q.tail)
		resume(q_dequeue(&cv->q));
	cv->test_and_set_lock = 0;
	return 0;
}

int test_and_set(volatile int *ts_var)
{
	intmask mask = disable();
	volatile int initial = *ts_var;
	*ts_var = 1;
	restore(mask);
	return initial;
}

void q_enqueue(int pid,qarray* queue)
{
	//kprintf("Enqueue\t%d\tNPROC = %d\n",queue->tail,NPROC);
	queue->q[queue->tail] = pid;
	queue->tail = (queue->tail + 1)%NPROC;
}

int q_dequeue(qarray* queue)
{
	//kprintf("Dequeue\n");
	int temp = queue->q[queue->head];
	queue->head = (queue->head+1)%NPROC;
	return temp;
}
