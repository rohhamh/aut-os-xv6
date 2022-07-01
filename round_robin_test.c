#include "types.h"
#include "stat.h"
#include "user.h"

int
be_child() {
  for (int i = 0; i < 1000; i++) {
    printf(1, "[PID]: %d", i);
  }
}

int
main(int argc, char *argv[])
{
  int pid;
  for (int i = 0; i < 10; i++) {
    pid = fork();
    if (pid == 0) {
      break;
    }
  }

  if (pid == 0) {
    be_child();
  }
  
  exit();
}
