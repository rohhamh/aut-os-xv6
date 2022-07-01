#include "types.h"
#include "stat.h"
#include "user.h"
#include "param.h"

#define CHILD_COUNT 30

void
be_child() {
  const int pid = getpid();
  for (int i = 0; i < 250; i++) {
    printf(1, "[%d]: %d\n", pid, i);
  }
  print_time_stats();
  return;
}

void test() {
  for (int i = 0; i < CHILD_COUNT; i++) {
    if (fork() == 0) {
      // int p = 6 - (i / 5);
      // printf(1, "Setting %d to %d\n", getpid(), p);
      // set_priority(p);
      be_child();
      exit();
    }
  }
  
  for (int i = 0; i < CHILD_COUNT; i++) {
    wait();
  }
  

  printf(1, "\n***PARENT***");
  print_time_stats();
}

int
main(int argc, char *argv[])
{
  change_policy(LOTTERY);
  test();
  exit();
}
