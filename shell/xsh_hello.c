/* xsh_hello.c - xsh_hello */

#include <xinu.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * xhs_hello - takes the argument and uses it to greet the user.
 *------------------------------------------------------------------------
 */
shellcmd xsh_hello(int nargs, char *args[])
{
	

	if (nargs > 1) {
		printf("Hello %s, Welcome to the world of Xinu!", args[1]);
	}
	else{
	printf("Please provide an argument...");	
	}

	printf("\n");

	return 0;
}
