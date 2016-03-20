
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<stdint.h>
void write_data(int file_descriptor, int32_t begin_seq,int32_t *sending_array,int offset)
{
	int32_t max_number = pow(2,20)-1,i,no_bytes;


	for(i = 0; i < max_number ; i++)
	{
		sending_array[i] = begin_seq + i + offset;	
	}

	//printf("Writing the array containing %d elements.\t Last element = %d\n",i,sending_array[i-1]);
	no_bytes = write(file_descriptor,sending_array,sizeof(int32_t)*max_number);

	if(no_bytes <0)
		perror("Write to file descriptor");
}

void read_data(int file_descriptor, int32_t *receiving_array){
	
	int32_t max_number = pow(2,20)-1,i;

	read(file_descriptor,receiving_array,sizeof(int32_t)*max_number);
	//printf("Received data\n");
	//for(i = 0; i < max_number ; i++)
	//	printf("%d\n",receiving_array[i]);
}

int verify_data(int32_t *sent_data_array, int32_t *acknowledged_data_array){
	int32_t max_number = pow(2,20)-1,i;
	int checker = 0;


	for(i = 0; i < max_number ; i++)
	{
		if(sent_data_array[i]+1 != acknowledged_data_array[i])
		{
			printf("Sending %d\t receiving %d at position %d\n",sent_data_array[i],acknowledged_data_array[i],i);
			checker = 1;
			break;
		}
			
	}

	return checker;
}




