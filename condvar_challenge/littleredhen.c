/* Copyright 2016 Rose-Hulman */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>

#define NUM_LOAVES_PER_BATCH 7
#define NUM_BATCHES 6
#define NUM_LOAVES_TO_EAT 14

/**
  This system has four threads: the duck, cat, and dog that eat 
  bread, and the little red hen that makes the bread. The little
  red hen makes seven loaves per batch, but she only has the patience
  for six batches. The little red hen only makes a batch if there are
  no loaves left.

  The other three animals each want to eat 14 loaves of
  bread, but only one of them can be in the kitchen at a time (to avoid
  fights over who gets what bread).

  When the duck, cat, or dog notices that there are no loaves of bread
  available, they complain to the little red hen and wait (in the kitchen)
  for the next batch to be ready.

  Use mutex locks and condition variables to enforce this constraint. Note: the
  global numLoaves variable should be left as is.

  look at littleRedHenSampleOutput.txt for an example correct output
  sequence

 **/

int numLoaves;
pthread_cond_t bakingBread;
pthread_cond_t eatingBread;
pthread_cond_t stayOut;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock3 = PTHREAD_MUTEX_INITIALIZER;


int animalInKitchen = 0;
int eating = 0;

void *littleRedHenThread(void *arg) {
 pthread_mutex_lock(&lock);
  char *name = (char*)arg;
  int batch;

  for (batch = 1; batch <= 6; batch++) {
    sleep(2);  // just makes it obvious that it won't work without
               // condition variables
    numLoaves += 7;
    printf("%-20s: A fresh batch of bread is ready.\n", name);
    pthread_cond_signal(&eatingBread);
    if(batch == 6){
	break;
    }
    while(numLoaves > 0){
     pthread_cond_wait(&bakingBread, &lock);
    }
  }

  printf("%-20s: I'm fed up with feeding you lazy animals! "
         "No more bread!\n", name);
 pthread_mutex_unlock(&lock);
  return NULL;
}

void *otherAnimalThread(void *arg) {
  char *name = (char*)arg;
  int numLoavesEaten = 0;
  while (numLoavesEaten < NUM_LOAVES_TO_EAT) {
  pthread_mutex_lock(&lock2);
  animalInKitchen++;
  if(animalInKitchen > 1){
    while(eating == 1){
     pthread_cond_wait(&stayOut, &lock2);
    }
  }
  eating++;
    if (numLoaves <= 0) {
      printf("%-20s: Hey, Little Red Hen, make some more bread!\n", name);
      pthread_cond_signal(&bakingBread);
      while(numLoaves <= 0){
        pthread_cond_wait(&eatingBread, &lock2);
      }
    }
    numLoaves--;
    printf("%-20s: Mmm, this loaf is delicious.\n", name);
    numLoavesEaten++;
 
    animalInKitchen--;
    eating--;
    pthread_cond_signal(&stayOut);
    pthread_mutex_unlock(&lock2);
    if (random() > random()) {  // Adds variety to output
      sleep(1);
    }
  }

  pthread_mutex_lock(&lock2);
  pthread_cond_broadcast(&stayOut);
  printf("%-20s: I've had my fill of bread. Thanks, Little Red Hen!\n", name);
  pthread_mutex_unlock(&lock2);
  return NULL;
}

int main(int argc, char **argv) {
  pthread_t dog, cat, duck, hen;
  numLoaves = 0;
  char dogName[] = "Lazy Dog";
  char catName[] = "Sleepy Cat";
  char duckName[] = "Noisy Yellow Duck";
  char henName[] = "Little Red Hen";

  pthread_create(&dog, NULL, otherAnimalThread, dogName);
  pthread_create(&cat, NULL, otherAnimalThread, catName);
  pthread_create(&duck, NULL, otherAnimalThread, duckName);
  pthread_create(&hen, NULL, littleRedHenThread, henName);
  pthread_join(dog, NULL);
  pthread_join(cat, NULL);
  pthread_join(duck, NULL);
  pthread_join(hen, NULL);

  printf("Everything finished.\n");
}
