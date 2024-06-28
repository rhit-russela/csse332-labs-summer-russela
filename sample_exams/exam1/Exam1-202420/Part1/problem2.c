#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

// Problem 2
//
// Name: 


// Default buffer size for use with snprintf
#define BUFSIZE 128
#define NUM_CHILDREN 10

int main(int argc, char **argv) {
  int i;
  pid_t pid = 0;
  int children_pids[NUM_CHILDREN];
  //create the children
  for(i = 0; i < NUM_CHILDREN; i++){
    //fork a new child
    int parent_pid = getpid();
    pid = fork();
    int currentpid = getpid();

    if(pid < 0){
      perror("PANIC");
      exit(1);
    }
    
    if(pid == 0){
      //child code

      int my_pid = getpid();
      
      if(my_pid % 4 == 0){
	printf("Child %d is bad, will not exec\n", my_pid);
	exit(89);
      }

      //time out 
      alarm(5);
      
      //change the int to a string
      char buff[BUFSIZE];
      snprintf(buff, BUFSIZE, "%d", parent_pid);
      
      execlp("./prettier.bin","./prettier.bin", buff, "Be brave and never give up!", NULL);
      perror("PANIC: exec failed");
	exit(1);
    }else{
      //parent code
      if(pid % 4 == 0){
	//crashes must skip
	waitpid(pid, 0, 0);
	i = i - 1; //retry the iteration to get successful child
      }else{
	children_pids[i] = pid; //getpid gets the parents pid
      }
    }
  }

  // done creating
  for(int j = 0; j < NUM_CHILDREN; j++){
    int status;
    waitpid(children_pids[j], &status, 0);
    if(WIFEXITED(status)){
      if(WEXITSTATUS(status) == 99){
	printf("Child %d timed out!\n", children_pids[j]);
      }else{
	printf("Child %d exited with exit status %d\n", children_pids[j], WEXITSTATUS(status));
      }
    }else{
      printf("Child %d crashed!\n", children_pids[j]);
    }
  }

  printf("Parent %d finished ....\n", getpid());
  exit(0);
}

