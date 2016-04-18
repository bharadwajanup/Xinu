#include <xinu.h>
#include <stdio.h>
#include <shm.h>


uint16 is_shm_exists(int16 key)
{
	uint16 i = 0;
	for(i=0;i<NSHMENT;i++)
	{
		if(shmtab[i].key == key)
		{
			return i;
		}
	}
	return -1;
}

uint16 shmget(int16 key, uint32 size, int16 shmflag)
{
	int16 i, ret_val;
	i = is_shm_exists(key);
	if(shmflag==SHM_FETCH && i == -1)
	{
		printf("Flag SHM_FLAG. Key %d doesn't exists", key);
		return -1;
	}
	if(shmflag == SHM_CREAT && i>=0)
	{
		printf("Flag is SHM_CREAT but SHM already exists at %d\n", i);
		return -1;
	}
	if(i>=0){
		//printf("Shared memory found at %d\n", i);
		return i;
	}
	
	shmtab[curr_size].key = key;
	shmtab[curr_size].refcount = 0;
	shmtab[curr_size].size = size;
	shmtab[curr_size].shmemory = getmem(size);
	ret_val = curr_size;
	curr_size = (curr_size+1)%NSHMENT;
	return ret_val;
}

char* shmat(int16 shmid, char* shmaddr, int16 shmflag)
{
	shmtab[shmid].refcount++;
	if((shmaddr == NULL) && (shmflag == SHM_RND))
	{
		return shmtab[shmid].shmemory;
	}
	else if((shmaddr!=NULL) && (shmflag == SHM_RND))
	{
		shmaddr = shmtab[shmid].shmemory;
		return shmaddr;
	}
	else if(shmflag == SHM_RDONLY)
	{
		return shmtab[shmid].shmemory;
	}
}

char* shmdt(int16 shmid, int16 shmflag)
{
	shmtab[shmid].refcount--;
	if(shmtab[shmid].refcount==0)
	{
		freemem(shmtab[shmid].shmemory, shmtab[shmid].size);
		shmtab[shmid].key = shmtab[shmid].size = -1;
		shmtab[shmid].shmemory = NULL;
		printf("Shared memory released\n");
	}
	return NULL;
}
