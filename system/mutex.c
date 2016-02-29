#include <xinu.h>
#include <babybird.h>

volatile int test_lock = 0, test_unlock = 0;
volatile int cond_lock = 0, cond_unlock = 0;
//volatile int wait_process_pid;
int test_and_set(volatile int*);

syscall mutex_create(mutex_t *lock){
	lock->value = 1;
	return 0;
}
/**
* Decrement value
* if <0
**/
syscall mutex_lock(mutex_t *lock){	
	while(test_and_set(&test_lock)==1)
		;
	//kprintf("Entered Critical section\t lock value %d\t address %x",lock->value, lock);
	//Critical section begins
	while(lock->value<=0)
	;
		
	lock->value=0;
	//kprintf("LOCKED\n");
	//test_lock = 0;
	//Critical section ends
	return 0;
}

syscall mutex_unlock(mutex_t *lock)
{
	//while(test_and_set(&test_unlock)==1)  
		//;
	lock->value=1;
	//kprintf("UNLOCKED\t lock value %d\n",lock->value);
	//lock->test_and_set_lock = 0;
	test_lock = 0;
	return 0;
}

syscall cond_init(cond_t *cv)
{
	cv->value = 0;
	return 0;
}

syscall cond_wait(cond_t *cv, mutex_t *lock)
{
	mutex_unlock(lock);
	while(test_and_set(&cond_lock)==1)  
		;
	/*
	//Critical section begins
	while(cv->value<=0){
		// /kprintf("STUCK\n");
	}
	//mutex_lock(lock);
	cv->value=0;
	cond_lock = 0;
*/

//Alternate implementation for conditional variables...

	cv->value = getpid();
	suspend(getpid());
	cond_lock = 0;
	return 0;
}

syscall cond_signal(cond_t *cv)
{
//	cv->value=1;
	
	resume(cv->value);
	cv->value = 0;
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
