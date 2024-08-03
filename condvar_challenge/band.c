/* Copyright 2021 Rose-Hulman */
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdbool.h>

/**
  Imagine a group of friends are getting together to play music, but
  they are arriving at different times.  Arriving can happen at any
  time (e.g. when some other friends are playing).

  There are 3 different kinds of friends - drummers, singers, and
  guitarists.  It takes one of each kind to make a band, plus only
  1 band can be playing at once.  Once those conditions are met, the
  players can start playing and stop playing in any order.  However,
  all 3 players must stop playing before a new set of 3 can start
  playing.

  Example output:

  drummer arrived
  drummer arrived
  guitarist arrived
  guitarist arrived
  singer arrived
  drummer playing
  guitarist playing
  singer playing
  singer arrived
  singer arrived
  drummer arrived
  guitarist arrived
  drummer finished playing
  guitarist finished playing
  singer finished playing
  singer playing
  guitarist playing
  drummer playing
  singer finished playing
  guitarist finished playing
  drummer finished playing
  guitarist playing
  drummer playing
  singer playing
  guitarist finished playing
  drummer finished playing
  singer finished playing
  Everything finished.


 **/

int DRUM = 0;
int SING = 1;
int GUIT = 2;

char* names[] = {"drummer", "singer", "guitarist"};

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock3 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t stageManager;
pthread_cond_t doorManager;

int band[] = {0, 0, 0};
int onStage = 0;



void bandCheckIn(int member){

 pthread_mutex_lock(&lock);
  if(band[member] == 1){
     while(band[member] == 1){
	pthread_cond_wait(&doorManager, &lock);
     }
     pthread_mutex_unlock(&lock); 
     bandCheckIn(member);
    }else{
      band[member] = 1;
      onStage++;
      if(onStage == 3){
        pthread_cond_broadcast(&stageManager);
        pthread_mutex_unlock(&lock);
        return;
      }
      while(onStage < 3){
       pthread_cond_wait(&stageManager, &lock);
      }
    }
 pthread_mutex_unlock(&lock);
}

void bandCheckOut(int member){
 pthread_mutex_lock(&lock2);
 if(band[member] == 1){
  band[member] = 0;
  onStage--;
 }

 if(onStage == 0){
  pthread_cond_broadcast(&doorManager);
 }

 pthread_mutex_unlock(&lock2);

}


// because the code is similar, we'll just have one kind of thread
// and we'll pass its kind as a parameter
void* friend(void * kind_ptr) {
  int kind = *((int*) kind_ptr);
  printf("%s arrived\n", names[kind]);

  bandCheckIn(kind);

  printf("%s playing\n", names[kind]);

  sleep(1);
  printf("%s finished playing\n", names[kind]);

  bandCheckOut(kind);

  return NULL;
}

pthread_t friends[100];
int friend_count = 0;

void create_friend(int* kind) {
  pthread_create(&friends[friend_count], NULL, friend, kind);
  friend_count++;
}

int main(int argc, char **argv) {

  create_friend(&DRUM);
  create_friend(&DRUM);
  create_friend(&GUIT);
  create_friend(&GUIT);
  sleep(1);
  create_friend(&SING);
  create_friend(&SING);
  create_friend(&DRUM);
  create_friend(&GUIT);
  create_friend(&SING);

  // all threads must be created by this point
  // note if you didn't create an equal number of each, we'll be stuck forever
  for (int i = 0; i < friend_count; i++) {
    pthread_join(friends[i], NULL);
  }

  printf("Everything finished.\n");

}
