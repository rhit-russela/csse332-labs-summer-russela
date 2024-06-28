#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

// Problem 1
//
// Name: Alyssa Russell

//waitpid can help determine who we are waiting for.

// Default buffer size for use with snprintf
#define BUFSIZE 128
#define NUM_CHILDREN 10

int main(int argc, char **argv) {
  pid_t pid = 0;
  int children_pids[NUM_CHILDREN];
  //create the children
  for(int i = 0; i < NUM_CHILDREN; i++){
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
      //change the int to a string
      char buff[BUFSIZE];
      snprintf(buff, BUFSIZE, "%d", parent_pid);
      
      execlp("./pretty.bin","./pretty.bin", buff, "Be brave and never give up!", NULL);
      perror("PANIC: exec failed");
	exit(1);
    }else{
      //parent code
	children_pids[i] = pid; //getpid gets the parents pid
    }
  }

  // done creating
  for(int j = 0; j < NUM_CHILDREN; j++){
    int status;
    waitpid(children_pids[j], &status, 0);
    if(WIFEXITED(status)){
      printf("Child %d exited with exit status %d\n", children_pids[j], WEXITSTATUS(status));
    }else{
      printf("Child %d crashed!\n", children_pids[j]);
    }
  }

  
  printf("Parent %d finished ....\n", getpid());
  exit(0);
}

