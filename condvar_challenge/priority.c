#include <stdio.h>
#include <pthread.h>
#include <unistd.h>


/**
  In this system there are threads numbered 1-6 and a critical
  section.  The thread numbers are priorities, where thread 6 is
  highest priority, thread 5 is next, etc.

  If the critical section is empty, any thread can enter.  While a
  thread is in the critical section no other threads can enter -
  regardless of priority. When a thread leaves the critical section
  and there are threads waiting, the highest priority waiting thread
  is allowed to enter the critical section.

  Be sure a newly arriving thread can't jump into the critical
  section as the current thread finishes, bypassing priority rules.
  Solve this problem with mutexes/condition variables
 **/

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t priority;

int waiting = 0;
int inCrit = 0;
int highestPriority = 0;

void *thread(void *arg)
{
  int *num = (int *) arg;
  printf("%d wants to enter the critical section\n", *num);

  pthread_mutex_lock(&lock);

  if(inCrit != 0){
    waiting ++;
  }

  while(waiting != 0){
   if(*num > highestPriority){
     highestPriority = *num;
     //printf("Highest: %d\n", highestPriority);
   }
   //printf("THREAD %d IS RUNNING\n", *num);
   while((*num != highestPriority && waiting > 1) || inCrit != 0){
     pthread_cond_wait(&priority, &lock);
   }
   //printf("THREAD %d IS RUNNING AFTER BROADCAST\n", *num);

   if(*num == highestPriority || waiting == 1){
      waiting--;
   //printf("NUM: %d is LEAVING\n", *num);
   //printf("WAITING LEFT: %d\n", waiting);
      break;
   }
  }

  printf("%d has entered the critical section\n", *num);
  inCrit++;
  pthread_mutex_unlock(&lock);

  sleep(1);

  pthread_mutex_lock(&lock);
  printf("%d is finished with the critical section\n", *num);
  inCrit--;
  pthread_cond_broadcast(&priority);
  pthread_mutex_unlock(&lock);

  return NULL;
}

int
main(int argc, char **argv)
{
  int i;
  pthread_t threads[6];
  int nums[] = {2, 1, 4, 3, 5, 6};


  for(i = 0; i < 6; i++) {
    pthread_create(&threads[i], NULL, thread, &nums[i]);

    if(i == 2) sleep(10);
  }

  for(i = 0; i < 6; i++) {
    pthread_join(threads[i], NULL);
  }

  printf("Everything finished.\n");
}
