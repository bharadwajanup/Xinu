#include <xinu.h>
#include <stdio.h>
#include <shm.h>

int16 curr_size = 0;

shellcmd xsh_shmdemo(int nargs, char *args[]){
	if(nargs<2)
	{
		printf("Please pass the string to be passed\n");
		return 0;
	}
	// printf("string to be passed %s\n",args[1]);
	sid32 writer_to_reader = semcreate(0);
	sid32 reader_to_writer = semcreate(0);
	resume(create(writer, 1024, 20, "Writer", 3, writer_to_reader, reader_to_writer, args[1]));
	resume(create(reader, 1024, 20, "Reader", 2, writer_to_reader, reader_to_writer));
	return 0;
}