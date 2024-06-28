#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

// Problem 4
//
// Name: 


// IMPORTANT: buffer size for use with ALL reads and writes
#define BUFSIZE 1000

int main(int argc, char **argv) {
  int rc;
  int fd[2];

  if(pipe(fd) < 0){
    perror("PANIC: pipe failed");
    exit(199);
  } //ALWAYS PIPE BEFORE YOU FORK;

  //parent is writer, child is reader!!!
  
  rc = fork();
  if(rc == 0){
    //child
    char readend[128], writeend[128];
    snprintf(readend, 128, "%d", fd[0]);
    snprintf(writeend, 128, "%d", fd[1]); // change int to a string
    execlp("./buffalosay.bin", "./buffalosay.bin", readend, writeend, NULL);
    perror("PANIC\n");
    exit(199);
  }

  //I am writer so close reading end
  close(fd[0]);

  //step 1: send secret handshake in 1000 characters
  char buffer[1000];
  memset(buffer, 0, 1000);

  snprintf(buffer, 1000, "%s", "secret handshake"); //prints the char into buffer

  // write secrete handshake
  write(fd[1], buffer, 1000);

  // write the message
  write(fd[1], argv[1], strlen(argv[1]));
  
  int status;
  wait(&status);
    if(WIFEXITED(status)){
      printf("buffalo say exited with exit code %d\n", WEXITSTATUS(status));
    }

  printf("Parent %d finished ....\n", getpid());
  exit(0);
}

