#include "types.h"
#include "stat.h"
#include "user.h"

int base = 0, limit = 5;

int be_thread() {
  // printf(1, "hello a: %d\n", a);
  base++;
  if (base == limit) {
    printf(1, "\nID [%d] => HIT LIMIT!\n", thread_id());
    exit();
  } else {
    int nul = 0;
    thread_creator((void*)&be_thread, (void*) &nul);
    int state = thread_join();
    int tid = thread_id();
    if (state != -1) {
      printf(1, "\nID [%d] => [SUCCESS] 0\n", tid);
    } else {
      printf(1, "\nID [%d] => [FAILED] -1\n", tid);
    }
    // printf(1, "\nID [%d][%d] => %d\n", tid, thread_id(), tjoin);
  }
  return 1;
}

int
main(int argc, char *argv[])
{
  int nul = 0;
  thread_creator((void*)&be_thread, (void*) &nul);
  exit();
}
