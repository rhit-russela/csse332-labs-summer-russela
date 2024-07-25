#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

// Constants in the code
#define FIRST_ROOM_CAPACITY 5
#define FIRST_ROOM_SERVICE_RATE 1

#define SECOND_ROOM_CAPACITY 2
#define SECOND_ROOM_SERVICE_RATE 2
#define WAITING_ROOM_CAPACITY 2

#define NUM_PEOPLE_ARRIVING 10

// capture the of customers that leave early
int num_left_customers = 0;
// TODO: Define other state of the world variables here
int num_firstRoom = 0;
int num_secondWaiting = 0;
int num_secondRoom = 0;

// TODO: Define your mutex locks and condition variables:
pthread_cond_t firstRoom;
pthread_cond_t secondRoom;

pthread_mutex_t firstLock;
pthread_mutex_t secondLock;

pthread_mutex_t firstLeftLock;
pthread_mutex_t secondLeftLock;


void *customer(void *arg)
{
  long int tid = (long int)arg;

  printf("[Customer %ld] Just arrived at first room...\n", tid);

  pthread_mutex_lock(&firstLock);
  num_firstRoom++;
  // printf("Number of Customers in the first Room: %d\n", num_firstRoom);
  while(num_firstRoom > FIRST_ROOM_CAPACITY){
    printf("[Customer %ld] Joined the waiting room for First room...\n", tid);
    pthread_cond_wait(&firstRoom, &firstLock);
  }
  pthread_mutex_unlock(&firstLock);
  
  // Enter the first room.
  printf("[Customer %ld] Entered first room...\n", tid);
  sleep(FIRST_ROOM_SERVICE_RATE);
  printf("[Customer %ld] Left first room...\n", tid);

  pthread_mutex_lock(&firstLeftLock);
  num_firstRoom--;
  //printf("Number of Customers LEFT in the first Room: %d\n", num_firstRoom);
  pthread_cond_signal(&firstRoom);
  pthread_mutex_unlock(&firstLeftLock);
  
  // You might want to check if you need to enter the waiting room here or leave
  // here...
  // if(some condition) {
  //   printf("[Customer %ld] Left unhapy because waiting room is full...\n", tid);
  //   return 0;
  // }

  // In case the customer wants to enter the waiting room, you might want to
  // print the following:
  // printf("[Customer %ld] Joined the waiting room for second room...\n", tid);

  //printf("~Customer %ld~ MADE IT TO THE SECOND ROOM???\n", tid);
  
  pthread_mutex_lock(&secondLock);
   if(num_secondRoom == SECOND_ROOM_CAPACITY && num_secondWaiting < WAITING_ROOM_CAPACITY){
    num_secondWaiting++;
    printf("[Customer %ld] Joined the waiting room for second room...\n", tid);
    while(num_secondRoom == SECOND_ROOM_CAPACITY){
      pthread_cond_wait(&secondRoom, &secondLock);
    }
    num_secondRoom++;
   }else if(num_secondRoom < SECOND_ROOM_CAPACITY){
    num_secondRoom++;
  }else{
    num_left_customers++;
    printf("[Customer %ld] Left unhappy because waiting room is full...\n", tid);
    pthread_mutex_unlock(&secondLock);
    return 0;
  }
  pthread_mutex_unlock(&secondLock);

  // Enter the second room
  printf("[Customer %ld] Entered second room...\n", tid);
  sleep(SECOND_ROOM_SERVICE_RATE);
  printf("[Customer %ld] Left second room...\n", tid);

  pthread_mutex_lock(&secondLeftLock);
  num_secondRoom--;
  pthread_cond_signal(&secondRoom);
  num_secondWaiting--;
  pthread_mutex_unlock(&secondLeftLock);

  // Done, time to leave...
  return 0;
}

int
main(int argc, char **argv)
{
  int i;
  long int tids[NUM_PEOPLE_ARRIVING];
  pthread_t threads[NUM_PEOPLE_ARRIVING];
  srand(time(0));

  for(i = 0; i < NUM_PEOPLE_ARRIVING; i++) {
    tids[i] = i + 1;
    pthread_create(&threads[i], 0, customer, (void*)tids[i]);
    if(!(i % 2))
      sleep(rand() % 2);
  }

  for(i = 0; i < NUM_PEOPLE_ARRIVING; i++) {
    pthread_join(threads[i], 0);
  }

  printf("Everything finished: %d customers left unhappy...\n",
         num_left_customers);
  exit(EXIT_SUCCESS);
}
