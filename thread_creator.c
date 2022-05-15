#include "types.h"
#include "stat.h"
#include "user.h"

#define PAGESIZE 4096

int 
thread_creator(void (*fn) (void *), void *arg) {
  void *fptr = malloc(2 * PAGESIZE);
  void *stack;

  if (fptr == 0) 
    return -1;

  int mod = (uint) fptr % PAGESIZE;

  stack = mod == 0 ? fptr : fptr + (PAGESIZE - mod);
  
  int thread_id = thread_create((void*) stack);

  if (thread_id < 0)
    printf(1, "thread creation failed\n");
  else if (thread_id == 0) {
    (fn)(arg);
    free(stack);
    exit();
  }

  return thread_id;
}


