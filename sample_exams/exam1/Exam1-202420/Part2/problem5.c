#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

// Problem 5
//
// Name: 

// IMPORTANT: buffer size for use with ALL reads and writes
#define BUFSIZE 1000

int main(int argc, char **argv) {
 int rc;
  int read_fd[2];
  int write_fd[2];

  if(pipe(read_fd) < 0){
    perror("PANIC: pipe failed");
    exit(199);
  } //ALWAYS PIPE BEFORE YOU FORK;

  if(pipe(write_fd) < 0){
    perror("PANIC");
    exit(199);
  }

  //parent is writer, child is reader!!!
  
  rc = fork();
  if(rc == 0){
    //child
    char read_fd0[128], read_fd1[128], write_fd0[128], write_fd1[128];
    snprintf(read_fd0, 128, "%d", read_fd[0]);
    snprintf(read_fd1, 128, "%d", read_fd[1]); // change int to a string
    snprintf(write_fd0, 128, "%d", write_fd[0]);
    snprintf(write_fd1, 128, "%d", write_fd[1]);
    
    execlp("./buffalopipe.bin", "./buffalopipe.bin", write_fd0, write_fd1, read_fd0, read_fd1, NULL);
    perror("PANIC\n");
    exit(199);
  }

  //I am writer on write_fd so close reading end
  close(write_fd[0]);
  //I am reader on read_fd so close writing end
  close(read_fd[1]);

  // step 1: send the msg to print
  char buffer[1000];
  memset(buffer, 0, 1000);
  snprintf(buffer, 1000, "%s", argv[1]);
  write(write_fd[1], buffer, 1000);

  // step 2: send secret handshake
  memset(buffer, 0, 1000);
  snprintf(buffer, 1000, "%s", "goodbye world");
  write(write_fd[1], buffer, 1000);

  // step 3: read what buffalosay has gen
  memset(buffer, 0, 1000);
  read(read_fd[0], buffer, 1000);
  printf("%s\n", buffer);
  
  int status;
  wait(&status);
    if(WIFEXITED(status)){
      printf("buffalo say exited with exit code %d\n", WEXITSTATUS(status));
    }
  printf("Parent %d finished ....\n", getpid());
  exit(0);
}

