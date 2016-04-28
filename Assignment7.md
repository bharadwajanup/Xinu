#Assignment - 7

##Task 1
* Modified process.h file to add a counter that holds the number of entries in prdesc array.
* When a file is opened, its descriptor is pushed into prdesc and the counter is incremented.
* When a file is closed, the counter is decremented.
* Upon process termination, all entries in prdesc are iteratively closed to close down any files that were left open.

