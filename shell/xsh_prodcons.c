/* xsh_prodcons.c - xsh_echo */

#include <xinu.h>
#include <stdio.h>
#include <string.h>
#include <future.h>

/*------------------------------------------------------------------------
 * xsh_prodcons - write argument strings to stdout
 *------------------------------------------------------------------------
 */
volatile int no_of_proc;

shellcmd xsh_prodcons(int nargs, char *args[])
{
    no_of_proc = 0;

    if(nargs!=2)
    {
      printf("Invalid number of arguments\n");
      return 1;
    }
    else if(strcmp(args[1],"-f")!=0)
    {
      printf("Invalid option %s\n",args[1]);
      return 1;
    }

	 future_t* f_exclusive,
          * f_shared,
          * f_queue;

  f_exclusive = future_alloc(FUTURE_EXCLUSIVE);
  f_shared    = future_alloc(FUTURE_SHARED);
  f_queue     = future_alloc(FUTURE_QUEUE);

  if((f_exclusive == NULL)||(f_shared==NULL)||(f_queue==NULL))
  {
    return 1;
  }

  //Test FUTURE_EXCLUSIVE
  resume( create(future_prod, 1024, 20, "fprod1", 1, f_exclusive) );
  resume( create(future_cons, 1024, 20, "fcons1", 1, f_exclusive) );
  
  

  //Test FUTURE_SHARED
  resume( create(future_cons, 1024, 20, "fcons2", 1, f_shared) );
  resume( create(future_cons, 1024, 20, "fcons3", 1, f_shared) );
  resume( create(future_cons, 1024, 20, "fcons4", 1, f_shared) ); 
  resume( create(future_cons, 1024, 20, "fcons5", 1, f_shared) );
  resume( create(future_prod, 1024, 20, "fprod1", 1, f_shared) );

  // Test FUTURE_QUEUE
  resume( create(future_cons, 1024, 20, "fcons6", 1, f_queue) );
  resume( create(future_cons, 1024, 20, "fcons7", 1, f_queue) );
  resume( create(future_cons, 1024, 20, "fcons8", 1, f_queue) );
  resume( create(future_cons, 1024, 20, "fcons9", 1, f_queue) );
  resume( create(future_prod, 1024, 20, "fprod3", 1, f_queue) );
  resume( create(future_prod, 1024, 20, "fprod4", 1, f_queue) );
  resume( create(future_prod, 1024, 20, "fprod5", 1, f_queue) );
  resume( create(future_prod, 1024, 20, "fprod6", 1, f_queue) );

  while(no_of_proc>0){
    ;
  }
  future_free(f_exclusive);
  future_free(f_shared);
  future_free(f_queue);
  printf("Exiting\n");
	return 0;
}
