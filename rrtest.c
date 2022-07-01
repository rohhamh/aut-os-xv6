#include "types.h"
#include "stat.h"
#include "user.h"
#include "param.h"

#define CHILD_COUNT 10

void
be_child() {
  const int pid = getpid();
  for (int i = 0; i < 1000; i++) {
    printf(1, "[%d]: %d\n", pid, i);
  }
  print_time_stats();
  printf(1, "\n%d\n", getTicks());
  return;
}

void test() {
  for (int i = 0; i < CHILD_COUNT; i++) {
    if (fork() == 0) {
      be_child();
      exit();
    }
  }
  
  for (int i = 0; i < CHILD_COUNT; i++) {
    wait();
  }
  

  printf(1, "\n***PARENT***\n");
  print_time_stats();
  printf(1, "\n%d\n", getTicks());
}

int
main(int argc, char *argv[])
{
  change_policy(ROUND_ROBIN_QUANTUM);
  test();
  change_policy(PREEMPTIVE_PRIORITY_SCHEDULING);
  test();
  exit();
}
