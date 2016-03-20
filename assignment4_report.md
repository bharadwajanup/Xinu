#Assignment 4
##Part 1:
The parent process forks two processes. The writer processes intially uses shmget function to create a new shared memory with size depending on the string passed by the user in command line. It gets the shared memory address by calling shmat(). The writer process attaches to the shared which increments the refcount by 1. The writer process writes to the shared memory and signals to reader process. The reader process uses shmget() to get shmid and shmat() to get shared memory address. The reader process reads from shared memory, prints to screen, detaches from shared memory and signals to the writer process so that it can unlink from shared memory. When reference count is 0, the shared memory is released and shmtab entry will be cleaned up.

##Part2
Using C, we implemented a program which can be found in assignment4-part2 folder of this repository. In this, we benchmark different types of Inter Process Communication between a parent and a forked child process. In our implementation, the child process created will act as the leader and the parent process will be the follower. Synchronization is achieved using named semaphores for shared memory and file methods of IPC and since pipes read call is blocking, it does not explicit synchronization. We time each method of IPC and display it on the console. After every bi-directional transfer, the program iterates over the data to verify they are correct. The verification happens at both leader and follower processes.

##Questions:
1)Is freebuff more efficient than freemem? (Chapters 10.1 to 10.7)
Ans: freebuff used in buffer pools mitigates memory deprivation and limits the unintentional deadlocks. Hence it is improves the performance of the processes as one processes won't be deprived of the staorage. In this, it is efficient.

2)Suppose a processor has support for paging. Describe paging hardware that can be used to protect a process's stack from access by other processes, even if demand paging is not implemented (i.e., all pages remain resident and no replacement is performed).

Pagetable converts from virtual address to physical address. If a process tries to access an address which is not allocated to it, then the address is not found in its page table which results in page fault interrupt. Hence it is memory protected.
