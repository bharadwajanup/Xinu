
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<stdint.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<semaphore.h>
#include<time.h>


#define LEADSEMNAME "/wrsemaphore"
#define FOLLOWSEMNAME "/rwsemaphore"



void file_bench_leader(sem_t *lead_sem, sem_t *follow_sem,int fd,int32_t *sending_array, int32_t *receiving_array ){
		int iteration_number,begin_seq,checker,bytes_read;
		long long elapsed_time=0,follower_time=0,avg_time=0;
		lead_sem = sem_open(LEADSEMNAME,0);
		follow_sem = sem_open(FOLLOWSEMNAME,0);
		struct timespec begin, end;

		if(lead_sem == SEM_FAILED || follow_sem == SEM_FAILED)
		{
			perror("semaphore failed");
			exit(0);
		}


		for(iteration_number = 0; iteration_number<10;iteration_number++){
			begin_seq = 2 * iteration_number;
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&begin);
			lseek(fd,0,SEEK_SET);
			write_data(fd,begin_seq,sending_array,0);
			sem_post(lead_sem);
			sem_wait(follow_sem);
			lseek(fd,0,SEEK_SET);
			read_data(fd,receiving_array);
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&end);

			elapsed_time+=((end.tv_sec*1000000000)+end.tv_nsec) - ((begin.tv_sec*1000000000)+begin.tv_nsec);

			checker = verify_data(sending_array,receiving_array);

			if(checker == 1)
				printf("Data not correct at leader\n");
			else
				printf("Data verified at leader\n");


		}

		avg_time = elapsed_time/10;
		//printf("Average time = %lld\n",avg_time);
		sem_wait(follow_sem);
		lseek(fd,0,SEEK_SET);
		bytes_read = read(fd,&follower_time,sizeof(long long));
		if(bytes_read<0)
			perror("Error in reading");

		printf("Total time %lld\n",(follower_time+avg_time));
		sem_post(lead_sem);



}



void piped_bench_leader(int fd[], int fd1[], int32_t *sending_array, int32_t *receiving_array){
	int iteration_number,begin_seq,checker;
	long long elapsed_time=0,follower_time=0,avg_time=0;
	struct timespec begin, end;
	close(fd[0]); //Close the input side of the pipe.


	for(iteration_number = 0; iteration_number<10;iteration_number++){
		begin_seq = 2 * iteration_number;

		clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&begin);
		write_data(fd[1],begin_seq,sending_array,0);

		
		close(fd1[1]);

		read_data(fd1[0],receiving_array);

		clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&end);

		elapsed_time+=((end.tv_sec*1000000000)+end.tv_nsec) - ((begin.tv_sec*1000000000)+begin.tv_nsec);

		checker = verify_data(sending_array,receiving_array);

		if(checker == 1)
			printf("Data not correct\n");
		else
			printf("Data verified\n");
	}

	avg_time = elapsed_time/10;
	read(fd1[0],&follower_time,sizeof(long long));

	printf("Total time %lld\n",(follower_time+avg_time));


}