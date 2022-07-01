#include "types.h"
#include "stat.h"
#include "user.h"
#include "param.h"

#define CHILD_COUNT 30

void be_child()
{
  const int pid = getpid();
  for (int i = 0; i < 250; i++)
  {
    printf(1, "[%d]: %d\n", pid, i);
  }

  return;
}

void test()
{
  int pids[CHILD_COUNT] = {0};
  int waitings[CHILD_COUNT];
  int bursts[CHILD_COUNT];
  int turnarounds[CHILD_COUNT];
  int priorities[CHILD_COUNT];
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
      int p = 6 - (i / 5);
      // printf(1, "Setting %d to %d\n", getpid(), p);
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
    waitings[i] = sleepingTime + readyTime;
    bursts[i] = runningTime;
    turnarounds[i] = waitings[i] + bursts[i];
    priorities[i] = priority;
    waitingTimeAverage += waitings[i];
    burstTimeAverage += bursts[i];
    turnaroundTimeAverage += turnarounds[i];
    waitingArrayAverage[priority] += waitingTime;
    burstArrayAverage[priority] += burstTime;
    turnaroundArrayAverage[priority] += turnaroundTime;
  }

  for (int i = 0; i < CHILD_COUNT; i++)
  {
    printf(1, "\nPID : %d | Priority : %d | Waiting : %d | CBT : %d | Turnaround : %d ", pids[i], priorities[i], waitings[i], bursts[i], turnarounds[i]);
  }

  waitingTimeAverage /= CHILD_COUNT;
  burstTimeAverage /= CHILD_COUNT;
  turnaroundTimeAverage /= CHILD_COUNT;
  for (int i = 1; i < 7; i++)
  {
    waitingArrayAverage[i] /= 5;
    burstArrayAverage[i] /= 5;
    turnaroundArrayAverage[i] /= 5;
    printf(1, "\nAverage Waiting Time For Priority %d : %d", i, waitingArrayAverage[i]);
    printf(1, "\nAverage CBT For Priority %d : %d", i, burstArrayAverage[i]);
    printf(1, "\nAverage Turnaround Time For Priority %d : %d", i, turnaroundArrayAverage[i]);
    printf(1, "\n------------------");
  }
  printf(1, "\n****************************************************************");
  printf(1, "\nTotal Results : \n");
  printf(1, "\nAverage Waiting Time : %d", waitingTimeAverage);
  printf(1, "\nAverage CBT : %d", burstTimeAverage);
  printf(1, "\nAverage Turnaround Time : %d", turnaroundTimeAverage);
}

int main(int argc, char *argv[])
{

  change_policy(PREEMPTIVE_PRIORITY_SCHEDULING);
  test();
  exit();
}
