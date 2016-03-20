#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<stdint.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<semaphore.h>
#include<time.h>


#define LEADSEMNAME "/wrsemaphore"
#define FOLLOWSEMNAME "/rwsemaphore"

void file_bench_follower(sem_t *lead_sem, sem_t *follow_sem,int fd,int32_t *sending_array, int32_t *receiving_array ){

		int iteration_number,begin_seq,checker;
		long long elapsed_time=0,avg_time=0,*follower_time;
		struct timespec begin, end;
		lead_sem = sem_open(LEADSEMNAME,0);
		follow_sem = sem_open(FOLLOWSEMNAME,0);

		for(iteration_number = 0; iteration_number<10;iteration_number++){
			begin_seq = 2 * iteration_number;
			sem_wait(lead_sem);
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&begin);
			lseek(fd,0,SEEK_SET);
			read_data(fd,receiving_array);
			lseek(fd,0,SEEK_SET);
			write_data(fd,begin_seq,sending_array,1);
			sem_post(follow_sem);
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&end);

			elapsed_time+=((end.tv_sec*1000000000)+end.tv_nsec) - ((begin.tv_sec*1000000000)+begin.tv_nsec);
			checker = verify_data(receiving_array,sending_array);
			
			if(checker == 1)
				printf("Data not correct at follower\n");
			else
				printf("Data verified at follower\n");

		}
		avg_time = elapsed_time/10;
		lseek(fd,0,SEEK_SET);
		follower_time = &avg_time;
		write(fd,follower_time,sizeof(long long));
		sem_post(follow_sem);
		sem_wait(lead_sem);
}


void piped_bench_follower(int fd[], int fd1[], int32_t *sending_array, int32_t *receiving_array){
	int iteration_number,begin_seq,checker;
	long long elapsed_time=0,avg_time=0,*follower_time;
	struct timespec begin, end;
	close(fd[1]); //Close the output side of the pipe.

		for(iteration_number = 0; iteration_number<10;iteration_number++){
			begin_seq = 2 * iteration_number;
			//end_seq = pow(2,20)-1+(2*iteration_number);
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&begin);
			read_data(fd[0],receiving_array);

			/*read(fd[0],receiving_array,sizeof(int32_t)*max_number);
			printf("Received data\n");*/


		/*for(i = 0; i < max_number ; i++)
		{
			sending_array[i] = receiving_array[i]+1;
		}*/
		

		close(fd1[0]);

		/*printf("Sending Data\n");
		write(fd1[1],sending_array,sizeof(int32_t)*max_number);*/

		write_data(fd1[1],begin_seq,sending_array,1);

		clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&end);

		elapsed_time+=((end.tv_sec*1000000000)+end.tv_nsec) - ((begin.tv_sec*1000000000)+begin.tv_nsec);

		checker = verify_data(receiving_array,sending_array);

		/*for(i = 0; i < max_number ; i++)
		{
			 if(sending_array[i] != receiving_array[i]+1)
			 {
			 	printf("Sending %d\t receiving %d at parent\n",sending_array[i],receiving_array[i]);
			 	checker = 1;
			 	break;
			 }
			
		}*/

		if(checker == 1)
			printf("Data not correct\n");
		else
			printf("Data verified\n");
	}

	avg_time = elapsed_time/10;
	follower_time = &avg_time;
	write(fd1[1],follower_time,sizeof(long long));
}