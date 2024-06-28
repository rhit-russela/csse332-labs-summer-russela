#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#include "helpers.h"

void step1(const char *op, const char *message) {
  //pr_error("Step 1 not implemented yet!\n");

  int pid = fork();

  if(pid == 0){ // child
    // exec process caesar.bin
    execlp("./caesar.bin", "./caesar.bin", op, message, NULL);
    pr_error("PANIC exec failed");
    exit(199);
    
  } // parent

  wait(0);
  pr_log("Parent done after successfully executing caesar cipher!\n");
  
}

void step2(const char *op, const char *message) {
  //pr_error("Step 2 not implemented yet!\n");
  int fd[2];

  if(pipe(fd) < 0){
    pr_error("PANIC PIPE FAILED");
    exit(199);
  }
  
  int pid = fork();

  if(pid == 0){ // child
   
    // translate read end into char
    char read_fd[128];
    snprintf(read_fd, 128, "%d", fd[0]);

    // exec caesar.bin on the op and msg
    execlp("./caesar.bin", "./caesar.bin", op, read_fd, NULL);
    pr_error("PANIC exec failed");
    exit(199);

  } // parent writes to pipe
  close(fd[0]);
  write(fd[1], message, 1000);
  
  wait(0);
  pr_log("Parent done after successfully executing caesar cipher!\n");

  
}

void step3(const char *op, const char *message) {
  //pr_error("Step 3 not implemented yet!\n");

   int fd[2];
   int back_fd[2];

  if(pipe(fd) < 0){
    pr_error("PANIC PIPE FAILED");
    exit(199);
  }

  if(pipe(back_fd) < 0){
    pr_error("PANIC PIPE FAILED");
    exit(199);
  }
  
  int pid = fork();

  if(pid == 0){ // child
   
    // translate read end into char
    char read_fd[128], write_fd[128];
    snprintf(read_fd, 128, "%d", fd[0]);
    snprintf(write_fd, 128, "%d", back_fd[1]);

    // exec caesar.bin on the op and msg
    execlp("./caesar.bin", "./caesar.bin", op, read_fd, write_fd, NULL);
    pr_error("PANIC exec failed");
    exit(199);

  } // parent writes to pipe the message
  close(fd[0]); // close sending pipe reading end
  close(back_fd[1]); //close recieving pipe writing end

  // send message
  write(fd[1], message, 1000);

  // read message back
  char buffer[1000];
  read(back_fd[0], buffer, 1000);

  pr_log("Parent recieved message back from the caesar program: %s\n", buffer);

  close(back_fd[0]);
  close(fd[1]);
  
  wait(0);
  pr_log("Parent done after successfully executing caesar cipher!\n");

}

int
main(int argc, char **argv) {
  char *message, *op;
  if(argc < 3) {
    pr_error("You did not invoke problem1.bin correctly!\n");
    exit(EXIT_FAILURE);
  }
  op = argv[1];
  message = argv[2];

  pr_log("Parent running step 1\n");
  step1(op, message);
  printf("\n\n");

  pr_log("Parent running step 2\n");
  step2(op, message);
  printf("\n\n");

  pr_log("Parent running step 3\n");
  step3(op, message);
  printf("\n\n");

  exit(EXIT_SUCCESS);
}
