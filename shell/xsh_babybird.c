#include <xinu.h>
#include <stdio.h>
#include <babybird.h>

volatile mutex_t  mut_lock;
volatile cond_t cv;
volatile int num_eat_worms = 0, num_baby_birds = 0, num_fetch_worms = 0;
volatile int worms_in_dish = 0;
volatile int dish_empty = 0;

shellcmd xsh_babybird(int nargs, char *args[])
{
	int i=0;
	if(nargs!=4){
		printf("Invalid arguments\n");
		exit(1);
	}

	num_eat_worms  = atoi(args[1]);
	num_baby_birds  = atoi(args[2]);
	num_fetch_worms = atoi(args[3]);

	if(num_fetch_worms==0 || num_baby_birds==0 || num_fetch_worms==0){
		printf("Invalid input\n");
		exit(1);
	}
	mutex_create(&mut_lock);
	cond_init(&cv);

	resume(create(parentbird, 1024, 20, "parent bird",0));
	for(i=0;i<num_baby_birds;i++){
		resume(create(childbird, 5024, 20, "child bird", 2, num_eat_worms, i));
	}
	return 0;
}

void parentbird(){
	//kprintf("Parent process\n");

	while(1==1){
		//kprintf("parent came in\n");
		mutex_lock(&mut_lock);
		if(dish_empty==1){
			//kprintf("parent came in %d\n",mut_lock.value);
			
			worms_in_dish = num_fetch_worms;
			kprintf("Parent filled dish with %d worms\n",worms_in_dish);
			dish_empty = 0;
			cond_signal(&cv);
			//mutex_unlock(&mut_lock);
		}
		mutex_unlock(&mut_lock);
	}
}

void childbird(int num_worms_local, int id){
	//kprintf("CHild process %d\n",id);
	while(num_worms_local!=0)
	{
		mutex_lock(&mut_lock);
		if(dish_empty==1){
			mutex_unlock(&mut_lock);
		}
		else{
			if(worms_in_dish==0){
				kprintf("Bowl is empty...\n");
				dish_empty=1;
				cond_wait(&cv, &mut_lock);
			}
			else{
				num_worms_local--;
				worms_in_dish--;
				kprintf("Baby bird %d ate 1 worm. There are now %d more worms to be eaten.\n",id, num_worms_local);
				//kprintf("Baby ID %d released the lock\n",id);
				mutex_unlock(&mut_lock);
			}
		}
	}
	kprintf("Baby bird %d exits with %d worms left in the bowl.\n",id, worms_in_dish);
}
