/* xsh_hello.c - xsh_hello */

#include <xinu.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * xhs_hello - takes the argument and uses it to greet the user.
 *------------------------------------------------------------------------
 */
shellcmd xsh_hello(int nargs, char *args[])
{
	
	printf("Number of args: %d\n",nargs);
	if (nargs == 2) {
		printf("Hello %s, Welcome to the world of Xinu!", args[1]);
	}
	else if(nargs > 2)
		printf("Please provide One argument.");	
	else{
	printf("Please provide an argument...");	
	}

	open(LFILE0,"hello.txt","n"); 
	close(LFILE0);
	open(LFILE0,"hello1.txt","n");//Open a file and not close it.

	printf("\n");

	return 0;
}
