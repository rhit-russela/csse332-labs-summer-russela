#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

// the number of follower threads
#define NUM_FOLLOWERS 6
// the number of shifts that the leader will take
#define NUM_LEADER_SHIFTS 5
// how long is the leader's shift at work
#define LEADER_SHIFT rand() % 5
// how long does the leader hang around for rest
#define LEADER_REST 1
// how long is the follower's shift at work
#define FOLLOWER_SHIFT 2

// state variables, you might need more than the one I define here
int num_followers_in_playground = 0;

int leaderPlay = 0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t linger;

// concurrency means

void *leader(void *arg)
{
  int i;

  for(i = 0; i < NUM_LEADER_SHIFTS; i++) {
    // do some management work...
    printf("Leader doing shift %d out of %d...\n", i+1, NUM_LEADER_SHIFTS);
    sleep(LEADER_SHIFT);

    pthread_mutex_lock(&lock2);
    printf("Leader entered the playground...\n");
    leaderPlay = 1;
    pthread_cond_broadcast(&linger);
    pthread_mutex_unlock(&lock2);

    sleep(LEADER_REST);

    pthread_mutex_lock(&lock2);
    printf("Leader has left the playground...\n");
    leaderPlay = 0;
    pthread_mutex_unlock(&lock2);
  }

  return 0;
}

void *follower(void *arg)
{
  int tid = *(int*)arg;

  while(1) {
    pthread_mutex_lock(&lock);
    printf("Follower (%d) doing work...\n", tid);
    pthread_mutex_unlock(&lock);

    sleep(FOLLOWER_SHIFT);

    pthread_mutex_lock(&lock);
    printf("Follower (%d) entering the playground...\n", tid);
    // if the follower goes into the playground, then print this
    if(leaderPlay == 0){
       num_followers_in_playground++;
       printf("Follower (%d) lingering in the playground with %d others...\n", tid,
              num_followers_in_playground-1);
       while(leaderPlay == 0){
       	pthread_cond_wait(&linger, &lock);
       }
       num_followers_in_playground--;
    }
    printf("Follower (%d) left the playground...\n", tid);
    pthread_mutex_unlock(&lock);
  }
}

int
main(int argc, char **argv)
{
  pthread_t threads[1 + NUM_FOLLOWERS];
  int follower_ids[NUM_FOLLOWERS];
  int i;
  srand(time(0));

  for(i = 0; i < NUM_FOLLOWERS; i++) {
    follower_ids[i] = i + 1;
    pthread_create(&threads[i], 0, follower, &follower_ids[i]);
  }
  pthread_create(&threads[NUM_FOLLOWERS], 0, leader, 0);

  // only wait for the leader since the follower never exit
  pthread_join(threads[NUM_FOLLOWERS], 0);

  printf("Leader finished, exiting even if followers are still there...\n");
  exit(EXIT_SUCCESS);
}
