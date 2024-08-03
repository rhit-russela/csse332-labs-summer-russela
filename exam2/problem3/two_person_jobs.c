#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdbool.h>

#define   JOBNUM      4
#define   THREADNUM   JOBNUM*2

//
// These constants represent the possible states that a job can be in.
//  UNSTARTED: represents a job that no one has grabbed and is still up for
//             grabs.
//  STARTED:   represents a job that has been started.
//  DONE:      represents a job that has completed.
//
//  TODO: Add any more state constant here if needed.
//        Hint: It is better if you add at least 2 more states.
//
#define   UNSTARTED   0
#define   STARTED     1
#define   DONE        2
#define   WAITING     3
#define   PARTIALLY   4

// TODO: Define state of the world variables here.
int jobs[JOBNUM]; // the set of jobs that we are to execute.

// TODO: Add condition variables and mutex locks needed here.
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t startJob;

void *worker(void *arg) {
  int *num = (int*)arg;
  int job  = -1;
  int i;

  pthread_mutex_lock(&lock);
  for(i = 0; i < JOBNUM; i++) {
    if(jobs[i] == UNSTARTED) {
      printf("Worker %d ready to work on job %d\n", *num, i);
      jobs[i] = WAITING;
      job = i;
        while(jobs[i] == WAITING){
         pthread_cond_wait(&startJob, &lock);
        }
      break;
    }else if(jobs[i] == WAITING) {
    // under some circumstances, it ought to also print
    printf("Worker %d helping on job %d\n", *num, i);
    pthread_cond_signal(&startJob);
    jobs[i] = STARTED;
    job = i;
    break;
    }
  }

  //
  // This should never happen if your solution is correct, but I added this here
  // to help you catch some mistakes that you might make. If your code works
  // correctly, you can remove this if statement alltogether.
  // On the other hand, if your code triggers this if statement, this means
  // something is wrong in your code and you need to revisit your solution.
  //
  if(job == -1) {
    printf("Something bad has happened for worker %d\n", *num);
    return 0;
  }

  printf("[Worker %d]: Starting job %d\n", *num, job);
  pthread_mutex_unlock(&lock);

  sleep(1);

  //
  // At this point, you might want to print one of two messages:
  // 1. Worker %d partially finished on job %d\n, *num, job
  // 2. Worked %d completely finished on job %d\n, *num, job
  //
  // Which statements you should print depends on which worker is exiting, so be
  // careful about his.
  //

  pthread_mutex_lock(&lock);
  if(jobs[job] == STARTED){
     jobs[job] = PARTIALLY;
     printf("Worker %d partially finished on job %d\n", *num, job);
  }else if(jobs[job] == PARTIALLY){
     jobs[job] = DONE;
     printf("Worker %d completely finished on job %d\n", *num, job);
  }
  // under some circumstances, it ought to alternatively print
  // printf("Worker %d partially finished on job %d\n", *num, job);
  pthread_mutex_unlock(&lock);
  return 0;
}

int
main(int argc, char **argv) {
  pthread_t threads[THREADNUM];
  int thread_ids[THREADNUM];
  int i;

  for(int i = 0; i < JOBNUM; i++) {
    jobs[i] = UNSTARTED;
  }

  for(i = 0; i < THREADNUM; i++) {
    thread_ids[i] = i;
    pthread_create(&threads[i], NULL, worker, &thread_ids[i]);
    if(i == 4) sleep(5); // makes two groups of workers
  }

  for(i = 0; i < THREADNUM; i++) {
    pthread_join(threads[i], NULL);
  }

  printf("Everyting finished...\n");
  return 0;
}
