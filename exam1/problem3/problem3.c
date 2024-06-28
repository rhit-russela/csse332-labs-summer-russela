#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>

// PLEASE DO NOT REMOVE THIS LINE
extern int is_good_child(int, int);

int
main(int argc, char **argv) {
  // PLEASE DO NOT REMOVE THIS LINE
  srand(time(NULL));

  int i = 1;
  int parent_pid = getpid();
  
  while(i > 0){
    int pid = fork();
    pid_t child_pid = getpid();
    if(pid == 0){ //child
      while(1);
    }// parent

    if(is_good_child(child_pid, 1) == 0){
      /* if(kill(child_pid, SIGTERM) < 0){ // continues terminating both the parent and child
	                                //even though the pid is specified
	perror("PANIC CHILD STILL HERE");
	}*/
      printf("Bad child %d, too bad!\n", child_pid);
    }else{
      printf("Good child %d found, will go into an infinite loop!\n", child_pid);
      printf("Parent %d found a good child %d, will go into an infinite loop!\n", parent_pid, child_pid);
      while(1);
    }
    
  }

  exit(0);
}
