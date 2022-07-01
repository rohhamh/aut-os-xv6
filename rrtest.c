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
 
  return;
}

void test() {
  
    int pids[CHILD_COUNT]={0};
    int waitings[CHILD_COUNT];
    int bursts[CHILD_COUNT];
    int turnarounds[CHILD_COUNT];
    int priority,runningTime,sleepingTime,terminationTime,creationTime,readyTime;
    int waitingTimeAverage=0;
    int burstTimeAverage=0;
    int turnaroundTimeAverage=0;
  for (int i = 0; i < CHILD_COUNT; i++) {
    if (fork() == 0) {
      be_child();
      exit();
    }
  }
  
  for (int i = 0; i < CHILD_COUNT; i++) {
     pids[i]=wait2(&priority,&runningTime,&sleepingTime,&terminationTime,&creationTime,&readyTime);
         waitings[i]=sleepingTime+readyTime;
            bursts[i]=runningTime;
            turnarounds[i]=waitings[i]+bursts[i];
            waitingTimeAverage+=waitings[i];
            burstTimeAverage+=bursts[i];
            turnaroundTimeAverage+=turnarounds[i];
  }
  

 for(int i=0;i<CHILD_COUNT;i++){
        printf(1,"\nPID : %d | Waiting : %d | CBT : %d | Turnaround : %d ",pids[i],waitings[i],bursts[i],turnarounds[i]);
    }
    
    waitingTimeAverage/=CHILD_COUNT;
    burstTimeAverage/=CHILD_COUNT;
    turnaroundTimeAverage/=CHILD_COUNT;
    printf(1,"\nAverage Waiting Time : %d",waitingTimeAverage);
    printf(1,"\nAverage CBT : %d",burstTimeAverage);
    printf(1,"\nAverage Turnaround Time : %d",turnaroundTimeAverage);
}

int
main(int argc, char *argv[])
{
  change_policy(ROUND_ROBIN_QUANTUM);
   
   test();
  exit();
}
