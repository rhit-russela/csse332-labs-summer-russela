#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_cond_t bullDoze;

int numberAtWall = 0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;

void brickWall(void){
  pthread_mutex_lock(&lock);
  numberAtWall++;
  while(numberAtWall > 3){
      pthread_cond_wait(&bullDoze, &lock);
    }
  pthread_mutex_unlock(&lock);
}

void freeThread(void){
  pthread_mutex_lock(&lock2);
  pthread_cond_signal(&bullDoze);
  numberAtWall--;
  pthread_mutex_unlock(&lock2);
}

void *thread(void *arg)
{
	char *letter = (char *)arg;
	printf("%c wants to enter the critical section\n", *letter);
	
	brickWall();
	
	printf("%c is in the critical section\n", *letter);
	
	sleep(1);
	
	printf("%c has left the critical section\n", *letter);

	freeThread();


	
	return NULL;
}

int
main(int argc, char **argv)
{
	pthread_t threads[8];
	int i;
	char *letters = "abcdefgh";

	for(i = 0; i < 8; ++i) {
		pthread_create(&threads[i], NULL, thread, &letters[i]);

		if(i == 4)
			sleep(4);
	}

	for(i = 0; i < 8; i++) {
		pthread_join(threads[i], NULL);
	}

	printf("Everything finished...\n");

	return 0;
}
