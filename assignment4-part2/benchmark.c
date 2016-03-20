#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<stdint.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<semaphore.h>
#include<sys/mman.h>
#define SNAME "/semaphore"
#define LEADSEMNAME "/wrsemaphore"
#define FOLLOWSEMNAME "/rwsemaphore"
#define SEM_PERM 0644
#define FILE_PERM 0666
#define SEM_INITIAL 0
#define SHAREDMEMNAME "/sharedmem"


int main(int argc ,char *args){
	
	int fd[2],fd1[2];
	pid_t childpid;
	long iteration_number ;

	int32_t max_number,i,checker=0;
	int32_t *sending_array;
	int32_t *receiving_array;
	int32_t begin_seq, end_seq;
	int file_bench_file,shared_memory_descriptor;
	sem_t * sem, *lead_sem,*follow_sem;



	iteration_number = 0;
	max_number = pow(2,20)-1+(2*iteration_number);

	pipe(fd);
	pipe(fd1);

	sem = sem_open(SNAME,O_CREAT,SEM_PERM,SEM_INITIAL);

	lead_sem = sem_open(LEADSEMNAME,O_CREAT,SEM_PERM,SEM_INITIAL);
	follow_sem = sem_open(FOLLOWSEMNAME,O_CREAT,SEM_PERM,SEM_INITIAL);



	sending_array = (int32_t*)calloc(max_number,sizeof(int32_t));
	receiving_array = (int32_t*)calloc(max_number,sizeof(int32_t));


	if((childpid = fork())==-1)
	{
		perror("Fork");
		exit(1);
	}

	file_bench_file = open("file.txt", O_RDWR|O_CREAT, FILE_PERM);

	if(file_bench_file<0)
		perror("Error opening the file");

	shared_memory_descriptor = shm_open(SHAREDMEMNAME,O_RDWR|O_CREAT,FILE_PERM);

	if(childpid == 0){

		//Child Process
		sem = sem_open(SNAME,0);


		piped_bench_leader(fd,fd1,sending_array,receiving_array);
		printf("IPC by pipes finished\n");
		//sleep(2);	

		file_bench_leader(lead_sem,follow_sem,file_bench_file,sending_array,receiving_array);
		printf("IPC by file finished\n");
		//sleep(2);
		file_bench_leader(lead_sem,follow_sem,shared_memory_descriptor,sending_array,receiving_array);
		printf("IPC by shared memory finished\n");
		
	}
	else{
		//Parent process
		piped_bench_follower(fd,fd1,sending_array,receiving_array);
		//sleep(2);
		file_bench_follower(lead_sem,follow_sem,file_bench_file,sending_array,receiving_array);
		//sleep(2);
		file_bench_follower(lead_sem,follow_sem,shared_memory_descriptor,sending_array,receiving_array);
	}
	sem_unlink(LEADSEMNAME);
	sem_unlink(FOLLOWSEMNAME);
	shm_unlink(SHAREDMEMNAME);
	close(file_bench_file);

	return 0;
}