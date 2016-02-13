/* xsh_hello.c - xsh_hello */

#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>
#include <process_ring.h>

//int pid[NUM_PROCESSES];
int num_rounds = 4;
/*------------------------------------------------------------------------
 * xhs_hello - takes the argument and uses it to greet the user.
 *------------------------------------------------------------------------
 */
shellcmd xsh_process_ring(int nargs, char *args[])
{
	int i=0;
	char opt;
	printf("Number of args: %d\n",nargs);
	if(nargs < 2){
		printf("No arguments passed. Continuing with default 4\n");
		num_rounds = 4;
  }
	else if (nargs == 2) {
		num_rounds = atoi(args[1]);
		opt = "C";
	}
  else
  {
		num_rounds = atoi(args[1]);
		opt = args[2][0];
  }
	if(num_rounds == 0)
	{
		printf("Invalid argument. Continuing with default 4\n");
		num_rounds = 4;
	}

	printf("Number of rounds is %d and number of process %d\n", num_rounds, NUM_PROCESSES);
  processes[0] = NUM_PROCESSES*num_rounds;
  master = 0;
	for(i=0;i<NUM_PROCESSES;i++)
	{
		switch(opt){
			case 'C': resume(create(process_ring, 1024, 20, "", 2, i, num_rounds));break;
			case 'I': resume(create(process_ring_infinite, 1024, 20-i, "", 2, i, num_rounds));break;
			case 'H': resume(create(process_ring_hang, 1024, 20, "", 2, i, num_rounds));break;
			case 'O': resume(create(process_ring_out, 1024, 20, "", 2, i, num_rounds));break; 
			default: 	resume(create(process_ring, 1024, 20, "", 2, i, num_rounds));
		}
		//resume(create(process_ring, 1024, 10, "", 2, i, num_rounds));//, (i+1)%NUM_PROCESSES));
		//resume(create(process_ring_infinite, 1024, 20, "", 2, i, num_rounds));//, (i+1)%NUM_PROCESSES));
	}
	/*for(i=0;i<NUM_PROCESSES;i++)
	{
		pid[i] = create(process_ring, 1024, 10, "", 2, i, num_rounds);
	}
  resume(pid[0]);*/
  
	return 0;
}
