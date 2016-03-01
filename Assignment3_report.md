#Assignment 3

**Note**: We implemented both busy wait and resume suspend method for both mutex and conditional variables implementation.

##Mutex Implementation

###Resume and suspend:
We implemented Test and Set method for protecting the critical section of mutex implementation. The flag is part of the mutex structure.

A process thread will acquire the lock by decrementing the value field of the mutex and continue executing the body of the program. Any other thread trying to enter the body would be put into the suspended state. The pid of each of the thread will be put into a queue.

Once a thread finishes its execution in the critical section, it will release the lock by incrementing the value field of the mutex. Now the process in the top of the queue will be resumed. Thus, we have followed the FIFO fashion.

###Busy wait:

A naive implementation where process threads will busy wait until the mutex is released.


##Conditional Variable implementation.

###Resume and Suspend

Any thread calling the wait function would release its mutex lock, enqueue its pid and set to suspended state. When a thread signals a condition variable, the queue is emptied and all the processes in the queue are resumed. The signal function works similar to a broadcast.

###Busy wait:

A naive implementation where the threads busy wait until a condition is met.

##Observation:
Resume and suspend method solves starvation(as the suspend and resume of preocesses are done in FIFO fashion) and is more efficient busy wait method.

##Program Flow:

The user inputs the worms to eat, number of baby birds and worms to fetch in the same order to the baby_bird shell command. A parent process and the specified number of child processes are spawned which try to acquire a mutex lock, signal the parent if the bowl is empty and wait for the signal from the parent.
The parent waits for a signal from a child process and fills the bowl with the specified number of worms and signals its baby bird processes to start eating again. While the parent bird process runs in an infinite loop, the baby bird processes exits once they have eaten the given number of birds.

##Solutions to questions:

1) Starvation is a condition where a process or thread is denied access to the necessary resources to completes its execution.

No. The given protocol is not starvation free. There could be a condition where one process sets busy as true but cannot come out as "me" will not be equal to "turn" since other processes canmodify the value from the inner while loop.

2) Deadlock is a condition where each process is waiting for an event which an other waiting process can cause thus resulting in a situation where no process can cause an event the other one is waiting for. Deadlock can occur with the following conditions:
1)Mutual Exclusion
2)Hold and wait
3)No preemption
4)Circular wait.

Yes. With this protocol, deadlock is unlikely to happen as the processes can starve but will not satisfy all the conditions required for a deadlock...

##Contribution##

Both us have equally contributed for all parts of this assignment.



