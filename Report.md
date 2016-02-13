#**Assignment 2:**

**Note: Code is in "assignment_2" branch.**

**Program_usage:**

Shell command on xinu: process_ring.

**Parameters**:
[OPTIONAL] Number of rounds
[OPTIONAL] Type of behavior simulation [O: out of order, C: correct order, I: Infinite behavior, H: hang behavior ]
Eg: process_ring 4 C //for correct version
    process_ring 4 O //for out of order
    process_ring 4 I //for infinite behaviour
    process_ring 4 H //for hang behaviour

By default, number of rounds will be 4 and the program will simulate the correct behavior('C').


**Implementation:**

The program creates "NUM_PROCESSES" number of processes and a process inbox is assigned to each process in the "processes" array (defined in process_ring.h). Each process created would execute the function defined in process_ring.c (The function could be one of the four defined in this file depending on the behavior we would like to simulate). The initial count is assigned to the process_inbox 0 before the function call is made.

**Program behavior:**

1) Out of Order output: 

The simplest implementation is each respective process would read the value in the process inbox, decrement the value and put it into the inbox of the next process. The last process would put the value to the inbox of the first process in a cyclic fashion. 

To get out of order, we spawned four processes and allowed them to print and decrement the values without any explicit synchronization. So multiplt processes were able to print and decrement mail box at same time.

Observation: Each process competed with each other to finish executing and ended up with out of order output. Here the processes entered a race condition to finish executing. A sample output is given below:

INPUT:

process_ring 4 O

OUTPUT:

Round: 0, Process: 0, Value 16
Round: 1, Process: 0, Value 16
Round: 2, Process: 0, Value 16
Round: 3, Process: 0, Value 16
Round: 0, Process: 1, Value 15
Round: 1, Process: 1, Value 15
Round: 2, Process: 1, Value 15
Round: 3, Process: 1, Value 15
Round: 0, Process: 2, Value 14
Round: 1, Process: 2, Value 14
Round: 2Round: 0, Process: 3, Value 13
Round: 1, Proce,xsh $ ss: 3, Value 13
Round: 2, Process: 3, Va lue 13
Round: 3, Process: 3, Value 13
Process: 2, Value 14
Round: 3, Process: 2, Value 14


2)Correct order output:

To ensure that the processes are executed in order, we introduced a master variable of type int which would indicate which process should run the loop and update the process_inbox. Once the process completes its execution loop, it will update the master variable and hand over the control to the next process. All the other processes simply waits untill the master variable reflects its mail_box index. With this, each process executes in order and hence the output of the program is in order and the programs counts down all the way till 1.

INPUT:

process_ring


OUTPUT:

Round: 0, Process: 0, Value 16
Round: 0, Process: 1, Value 15
Round: 0, Process: 2, Value 14
Round: 0, Process: 3, Value 13
xsh $ Round: 1, Process: 0, Value 12
Round: 1, Process: 1, Value 11
Round: 1, Process: 2, Value 10
Round: 1, Process: 3, Value 9
Round: 2, Process: 0, Value 8
Round: 2, Process: 1, Value 7
Round: 2, Process: 2, Value 6
Round: 2, Process: 3, Value 5
Round: 3, Process: 0, Value 4
Round: 3, Process: 1, Value 3
Round: 3, Process: 2, Value 2
Round: 3, Process: 3, Value 1


3) Hang while execution:

The processes can sometimes wait indefinitely to get the control over execution and thus result in program getting hanged. One of the reasons it could happen is when the master variable is not updated properly. So the processes will never get their turn to execute and hence wait indefinitely. The processes thus enter a deadlock condition.

INPUT:

process_ring 4 H


OUTPUT:

Round: 0, Process: 0, Value 16
Round: 1, Process: 0, Value 16
Round: 2, Process: 0, Value 16
Round: 3, Process: 0, Value 16


4) Infinite loop:

The processes enter an infinite loop when the exit condition is never achieved. Thus the processses continue to run indefinitely and never terminate. In our program, if the exit condition is not satisfied, the processes will go into infinite loop as they can never exit. 

INPUT:

process_ring 4 I

OUTPUT:


**Program runs indefinitely **


Observation: As processes schedule is controller entirely by operating systems and cannot be repdecited by end users, sometimes, the processes were behaving in erratic way. We understand due to this erratic nature of processes, we need process synchronization.



#QUESTIONS
1)Why does your program hang? What is the name of this behavior?

The program hangs as the processes are waiting to consume the value produced by predecessor process. But as the 'master' value is not updated, the processes can never access their mail box. When program hangs it is usually known as deadlock.

2)Why does your program go into an infinite loop? What is the name of this behavior?

Program goes to infinite loop due to improper synchronization among the processes. Due to improper process synchronization, processes never meet the exit condition, hence the processes never exit. This causes unbounded context switching.

3)Why does your program print numbers out of order? What is the name of this behavior?

All processes are trying to access the common resource, the 'mailbox array'. As multiple processes are trying accesses at same time and trying to change mail box value and print them, the print is out of order. This condition is known as race condition.

4)What property of Xinu allows the working version of your program to print values in the correct order?

We implemented busy waiting in order to print the values in correct order. In busy waiting, process will print and decrement the value only if it is its turn else it will wait until its chance arrives.

