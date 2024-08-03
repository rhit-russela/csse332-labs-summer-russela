/* Copyright 2019 Rose-Hulman */
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>


// number of carpenters
#define NUM_CARP 3
// number of painters
#define NUM_PAIN 3
// number of decorators
#define NUM_DECO 3

/**
  Imagine there is a shared memory space called house.

  There are 3 different kinds of operations on house: carpenters, painters, and
  decorators.  For any particular kind of operation, there can be an unlimited
  number of threads doing the same operation at once (e.g. unlimited carpenter
  threads etc.).  However, only one kind of operation can be done at a time (so
  even a single carpenter should block all painters and vice versa).

  Use mutex locks and condition variables to enforce this constraint.  You don't
  have to worry about starvation (e.g. that constantly arriving decorators might
  prevent carpenters from ever running) - though maybe it would be fun to
  consider how you would solve in that case.

  This is similar to the readers/writers problem BTW.
 **/

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock3 = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t carpenterLock;
pthread_cond_t painterLock;
pthread_cond_t decoratorLock;

void* carpenter(void * ignored) {
  pthread_mutex_lock(&lock);
  //pthread_cond_wait(&painterLock, &lock2);
  //pthread_cond_wait(&decoratorLock, &lock3);
  printf("starting carpentry\n");
  pthread_mutex_unlock(&lock);

  sleep(1);

  pthread_mutex_lock(&lock);
  printf("finished carpentry\n");
  //pthread_cond_broadcast(&painterLock);
  //pthread_cond_broadcast(&decoratorLock);
  pthread_mutex_unlock(&lock);
  return NULL;
}

void* painter(void * ignored) {
  pthread_mutex_lock(&lock2);
  //pthread_cond_wait(&carpenterLock, &lock);
  //pthread_cond_wait(&decoratorLock, &lock3);
  printf("starting painting\n");
  pthread_mutex_unlock(&lock2);

  sleep(1);

  pthread_mutex_lock(&lock2);
  printf("finished painting\n");
  //pthread_cond_broadcast(&carpenterLock);
  //pthread_cond_broadcast(&decoratorLock);
  pthread_mutex_unlock(&lock2);
  return NULL;
}

void* decorator(void * ignored) {
  pthread_mutex_lock(&lock3);
  //pthread_cond_wait(&painterLock, &lock2);
  //pthread_cond_wait(&carpenterLock, &lock);
  printf("starting decorating\n");
  pthread_mutex_unlock(&lock3);
  sleep(1);

  pthread_mutex_lock(&lock3);
  printf("finished decorating\n");
  //pthread_cond_broadcast(&painterLock);
  //pthread_cond_broadcast(&carpenterLock);
  pthread_mutex_unlock(&lock3);
  return NULL;
}


int main(int argc, char **argv) {
  pthread_t jobs[NUM_CARP + NUM_PAIN + NUM_DECO];
  for (int i = 0; i < NUM_CARP + NUM_PAIN + NUM_DECO; i++) {
    void* (*func) (void*) = NULL;
    if(i < NUM_CARP)
      func = carpenter;
    if(i >= NUM_CARP && i < NUM_CARP + NUM_PAIN)
      func = painter;
    if(i >= NUM_CARP + NUM_PAIN) {
      func = decorator;
    }
    pthread_create(&jobs[i], NULL, func, NULL);
  }

  for (int i = 0; i < NUM_CARP + NUM_PAIN + NUM_DECO; i++) {
    pthread_join(jobs[i], NULL);
  }

  printf("Everything finished.\n");

}
