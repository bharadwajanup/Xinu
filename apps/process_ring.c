#include <xinu.h>
#include <process_ring.h>
#include <stdio.h>




/***
** Correct version
***/
void process_ring(int mail_box, int num_rounds)
{
 int i=0;
 for(i=0;i<num_rounds;i++)
	{
  	while(master != mail_box);
    kprintf("Round: %d, Process: %d, Value %d\n",i,mail_box, processes[mail_box]);
    processes[(mail_box+1)%NUM_PROCESSES] = processes[mail_box]-1;
    master = (master+1)%NUM_PROCESSES;
  }
}


/***
** Out of order version
***/
void process_ring_out(int mail_box, int num_rounds)
{
 int i=0;
 for(i=0;i<num_rounds;i++)
	{
    printf("Round: %d, Process: %d, Value %d\n",i,mail_box, processes[mail_box]);
    processes[(mail_box+1)%NUM_PROCESSES] = processes[mail_box]-1;
  }
}
	
/***
** Hang version
***/
void process_ring_hang(int mail_box, int num_rounds)
{
 int i=0;
 for(i=0;i<num_rounds;i++)
	{
  	while(master != mail_box);
    kprintf("Round: %d, Process: %d, Value %d\n",i,mail_box, processes[mail_box]);
    processes[(mail_box+1)%NUM_PROCESSES] = processes[mail_box]-1;
  }
}

/***
** Infinite Loop
***/
void process_ring_infinite(int mail_box, int num_rounds)
{
 int i=0;
 while(processes[mail_box]!=(0-mail_box))
	{
    if(processes[mail_box]!=(0-mail_box)){
		  printf("Round: %d, Process: %d, Value %d\n",i,mail_box, processes[mail_box]);
		  processes[(mail_box+1)%NUM_PROCESSES] = processes[mail_box]-1;
   }
  }
}
		
	
