#include <xinu.h>
#include <stdio.h>
#include <future.h>

extern int no_of_proc;

uint16 future_prod(future_t* fut) {
  int i, j;
  j = (int)fut;
  no_of_proc++;

  for (i = 0; i < 1000; ++i) {
    j += i;
  }
  future_set(fut, &j);
  no_of_proc--;
  return OK;
}

uint16 future_cons(future_t* fut) {
  int i, status;
  no_of_proc++;
  status = (int)future_get(fut, &i);
  if (status < 1) {
    printf("future_get failed\n");
    no_of_proc--;
    return -1;
  }
  kprintf("it produced %d\n", i);
  no_of_proc--;
  return OK;
}