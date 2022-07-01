#include "types.h"
#include "stat.h"
#include "user.h"
#include "param.h"

#define CHILD_COUNT 60

void be_child()
{
  const int pid = getpid();
  for (int i = 0; i < 600; i++)
  {
    printf(1, "[%d]: %d\n", pid, i);
  }
  print_time_stats();
  return;
}

void test()
{

  int pids[60] = {0};
  int waitings[60];
  int bursts[60];
  int turnarounds[60];
  int priorities[60];
  int priority, runningTime, sleepingTime, terminationTime, creationTime, readyTime;
  int waitingTimeAverage = 0;
  int burstTimeAverage = 0;
  int turnaroundTimeAverage = 0;
  int waitingArrayAverage[7] = {0};
  int burstArrayAverage[7] = {0};
  int turnaroundArrayAverage[7] = {0};

  for (int i = 0; i < CHILD_COUNT; i++)
  {
    if (fork() == 0)
    {
      int p = (i / 10) + 1;
      printf(1, "Setting %d to %d\n", getpid(), p);
      set_priority(p);
      be_child();
      exit();
    }
  }

  for (int i = 0; i < CHILD_COUNT; i++)
  {
    pids[i] = wait2(&priority, &runningTime, &sleepingTime, &terminationTime, &creationTime, &readyTime);
    int waitingTime = sleepingTime + readyTime;
    int burstTime = runningTime;
    int turnaroundTime = waitingTime + burstTime;
    waitings[i] = waitingTime;
    bursts[i] = burstTime;
    turnarounds[i] = waitingTime + burstTime;
    priorities[i] = priority;
    waitingTimeAverage += waitingTime;
    burstTimeAverage += burstTime;
    turnaroundTimeAverage += turnaroundTime;
    waitingArrayAverage[priority] += waitingTime;
    burstArrayAverage[priority] += burstTime;
    turnaroundArrayAverage[priority] += turnaroundTime;
  }

  for (int i = 0; i < 60; i++)
  {
    printf(1, "\nPID : %d | Layer : %d | Waiting : %d | CBT : %d | Turnaround : %d ", pids[i], priorities[i], waitings[i], bursts[i], turnarounds[i]);
  }
  waitingTimeAverage /= 60;
  burstTimeAverage /= 60;
  turnaroundTimeAverage /= 60;
  for (int i = 1; i < 7; i++)
  {
    waitingArrayAverage[i] /= 10;
    burstArrayAverage[i] /= 10;
    turnaroundArrayAverage[i] /= 10;
    printf(1, "\nAverage Waiting Time For Layer %d : %d", i, waitingArrayAverage[i]);
    printf(1, "\nAverage CBT For Layer %d : %d", i, burstArrayAverage[i]);
    printf(1, "\nAverage Turnaround Time For Layer %d : %d", i, turnaroundArrayAverage[i]);
    printf(1, "\n------------------");
  }
  printf(1, "\n****************************************************************");
  printf(1, "\nTotal Results : \n");
  printf(1, "\nAverage Waiting Time : %d", waitingTimeAverage);
  printf(1, "\nAverage CBT : %d", burstTimeAverage);
  printf(1, "\nAverage Turnaround Time : %d\n", turnaroundTimeAverage);
}
int main(int argc, char *argv[])
{
  change_policy(PREEMPTIVE_MULTILEVEL_QUEUE);
  test();
  exit();
}
