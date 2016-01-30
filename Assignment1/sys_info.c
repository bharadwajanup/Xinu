#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{ 
	int pid, pipe_res, status;
	int fd[2];
	char buffer[80], command_path[80];

	if(argc!=2){
		printf("USAGE: ./sys_info {command}\n");  //Supports only one argument
		return 1;
	}
	printf("parent process id %d\n", getpid());  //Display parent process PID
	
	pipe_res = pipe(fd);

	if(pipe_res!=0){
		printf("PIPE failure\n");
	}
	pid = fork();

	if(pid == 0){
		printf("Child process ID %d\n",getpid());  //Display child process PID
		close(fd[1]);
		read(fd[0], buffer, sizeof(buffer));        
		printf("received argument %s\n",buffer);
		sprintf(command_path, "/bin/%s",buffer);

		if(strcmp(buffer,"echo")==0){              //Pass "hello world" for echo
			if(execl(command_path, buffer, "hello world", NULL)==-1){
			perror("exec failed\n");
		 }
		}

		if(execl(command_path, buffer, NULL)==-1){
			perror("exec failed\n");
		}
	}
	else {
		close(fd[0]);
		write(fd[1], argv[1], strlen(argv[1])+1);
		wait(&status);                              //parent process waits for child process to exit
	}
}