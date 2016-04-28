#Assignment - 7

##Task 1
* Modified process.h file to add a counter that holds the number of entries in prdesc array.
* When a file is opened, its descriptor is pushed into prdesc and the counter is incremented or if there are areas in the array which has been unused (with value -1), the new descriptor will occupy that place.
* When a file is closed, The corresponding descriptor in the array is removed from the array and set to -1.
* Upon process termination, all entries in prdesc are iteratively closed to close down any files that were left open.

##Task 2

Given data:
  * 16 data block pointers in an index block
  * 7 index block fill a disk block
  * Total number of disk blocks in the given disk: n
  * Number of files held by the disk: k

Now, one index block can hold 16 block pointers. Therefore, for k files we would need ceiling of (k/16) index blocks.
Therefore, total number of disk blocks occupied by file index blocks in the disk = (k/(16*7))

Each index block can have 16 data block pointers and seven such index blocks add up to one disk block. Therefore total number of disk blocks occupied by index blocks is: (summation) i=1 to i=k : (p(i)/(16*7)).

 where p(i) is the number of data blocks occupied by the ith file.

Now, total number of disk blocks 'n' can be written as an equation:


(k/(16 * 7)) + (summation) i=1 to i=k (p(i)/(16 * 7)) + (summation) i=1 to i=k p(i) = n




Therefore,

Total number of disk blocks occupied by index blocks = 

(k/(16 * 7)) + (summation) i=1 to i=k (p(i)/(16 * 7)) = n - (summation) i=1 to i=k p(i)


Credits: The solution to task 2 was discussed with Raghavendra and Chris on his AI office hours.








