#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_cond_t barrier;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int threadNext = 1;

void *thread(void *arg)
{
  int *num = (int *)arg;
  printf("%d wants to enter the critical section\n", *num);
  pthread_mutex_lock(&lock);
  while(threadNext != num[0]){
    pthread_cond_wait(&barrier, &lock);
  }
  pthread_cond_broadcast(&barrier);
  threadNext++;
  pthread_mutex_unlock(&lock);
  printf("%d is finished with the critical section\n", *num);

  return NULL;
}

int
main(int argc, char **argv)
{
  pthread_t threads[4];
  int i;
  int nums[] = {2, 1, 4, 3};

  for(i = 0; i < 4; ++i) {
    pthread_create(&threads[i], NULL, thread, &nums[i]);

    if(i == 2)
      sleep(3);
  }

  for(i = 0; i < 4; ++i) {
    pthread_join(threads[i], NULL);
  }

  printf("Everything finished\n");

  return 0;
}
