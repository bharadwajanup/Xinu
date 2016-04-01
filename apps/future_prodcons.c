#include <xinu.h>
#include <stdio.h>
#include <future.h>

uint16 future_prod(future_t* fut) {
  int i, j;
  //printf("came to prod 111 \n");
  // /printf("In future_prod, future mode is %d and value is %d\n", fut->mode, fut->value);
  j = (int)fut;
  for (i = 0; i < 1000; ++i) {
    j += i;
  }
  future_set(fut, &j);
  return OK;
}

uint16 future_cons(future_t* fut) {
  int i, status;
  //printf("came to cons\n");
  //printf("In future_cons, future mode is %d and value is %d\n", fut->mode, fut->value);
  status = (int)future_get(fut, &i);
  if (status < 1) {
    printf("future_get failed\n");
    return -1;
  }
  kprintf("%d consumed %d\n", getpid(), i);
  return OK;
}