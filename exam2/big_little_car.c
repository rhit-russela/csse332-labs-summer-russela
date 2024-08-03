/* Copyright 2020 Rose-Hulman */
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// half are big half are small
#define THREADNUM 8

// TODO: Define state of the world variables here, if needed.
int currentWeight = 0;

int maxWeight = 2;
int smallCar = 1;
int bigCar = 2;
// TODO: Define condition variables and mutex locks here, if needed.
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t waitYOURTURN;

void *bigcar(void *ignored) {
  printf("Big car approaching bridge\n");

  pthread_mutex_lock(&lock);
  if(currentWeight == 0){
    currentWeight = currentWeight + bigCar;
  }else{
    while((currentWeight + bigCar) > maxWeight){
	pthread_cond_wait(&waitYOURTURN, &lock);
    }
    currentWeight = currentWeight + bigCar;
  }
  //printf("STATE OF THE WORLD CURRENT WEIGHT %d\n", currentWeight);
  printf("\t -----> Big car starting bridge\n");
  pthread_mutex_unlock(&lock);

  sleep(1);

  pthread_mutex_lock(&lock);
  printf("\t -----> Big car finishing bridge\n");
  currentWeight = currentWeight - bigCar;
  //printf("STATE OF THE WORLD CURRENT WEIGHT %d\n", currentWeight);
  pthread_cond_broadcast(&waitYOURTURN);
  pthread_mutex_unlock(&lock);
  return 0;
}

void *littlecar(void *ignored) {
  printf("Little car approaching bridge\n");

  pthread_mutex_lock(&lock2);
  if(currentWeight < maxWeight){
    currentWeight = currentWeight + smallCar;
  }else{
    while((currentWeight + smallCar) > maxWeight){
	pthread_cond_wait(&waitYOURTURN, &lock2);
    }
   currentWeight = currentWeight + smallCar;
  }
  printf("\t -----> Little car starting bridge\n");
  pthread_mutex_unlock(&lock2);

  sleep(1);

  pthread_mutex_lock(&lock2);
  printf("\t -----> Little car finishing bridge\n");
  currentWeight = currentWeight - smallCar;
  //printf("STATE OF THE WORLD CURRENT WEIGHT %d\n", currentWeight);
  pthread_cond_broadcast(&waitYOURTURN);
  pthread_mutex_unlock(&lock2);
  return 0;
}

int main(int argc, char **argv) {
  pthread_t threads[THREADNUM];
  int thread_ids[THREADNUM];
  int i;

  for(i = 0; i < THREADNUM; i++) {
    thread_ids[i] = i;
    if (i % 2 == 0) {
      pthread_create(&threads[i], NULL, bigcar, &thread_ids[i]);
    } else {
      pthread_create(&threads[i], NULL, littlecar, &thread_ids[i]);
    }
    if(i == 4) sleep(5); // makes two groups of cars
  }

  for(i = 0; i < THREADNUM; i++) {
    pthread_join(threads[i], NULL);
  }

  printf("Everything finished.\n");
  return 0;
}
