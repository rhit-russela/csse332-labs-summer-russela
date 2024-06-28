#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#include "helpers.h"


/**
 * Feel free to use this function if needed, it is the same one as we used in
 * class.
 */
void setsighandler(int signum, void (*handler)(int)) {
  struct sigaction act;

  act.sa_handler = handler;
  sigemptyset(&act.sa_mask);
  act.sa_flags = SA_RESTART;
  sigaction(signum, &act, NULL);
}


void handle_fire(int signum){
  //donothing
}

/**
 * handle the infantry command
 */
void
infantry() {
  // TODO: Add your code for the infantry command here...
  int pid = fork();
  int childpid = getpid();

  signal(SIGKILL, 0);
  if(pid == 0){ // child
    printf("Infantry unit %d spawned, will be stationed and ready!\n", childpid);
    while(1);
  }// parent
  
  
}

volatile int hit_count = 0;

void handle_kill(int signum){
  hit_count++;
}

/**
 * handle the cavalry command
 */
void
cavalry() {
  // TODO: Add your code for the cavalry command here...
  int pid = fork();
  int childpid = getpid();

  signal(SIGKILL, handle_kill);
  if(pid == 0){ // child
    printf("Cavalry unit %d spawned, will be stationed and ready!\n", childpid);
    while(1){  
      if(hit_count > 3){
        exit(1);
      }
    }
  }// parent
}


/**
 * handle the armored command
 */
void
armored() {
  // TODO: Add your code for the armored command here...
  int pid = fork();
  int childpid = getpid();

  signal(SIGKILL, 0);
  if(pid == 0){ // child
    printf("Armored unit %d spawned, will be stationed and ready!\n", childpid);
    while(1);
  }// parent
}

/**
 * handle the fire command with a given pid
 */
void
fire(int pid) {
  // TODO: Add your code for the fire command here...
  kill(pid, SIGKILL);
}

/**
 * handle the list command
 */
void
list() {
  // TODO: Add your code for the list command here...
  int pid = fork();
  char* arg1 = "afo";
  char* arg2 = "pid,command";
  
  if(pid == 0){ // child
    execlp("/usr/bin/ps", "/usr/bin/ps", arg1, arg2, NULL);
    perror("PANIC EXEC FAIL");
    exit(199);
  }// parent

  wait(0);
  
}

/**
 * handle the dezombify command with a given pid
 */
void
dezombify(int pid) {
  // TODO: Add your code for the dezombify command here...
  waitpid(pid, 0, 0);
}

/*
 * handle the support command with a given pid
 */
void
support(int pid) {
  // TODO: Add your code for the support command here...
}

/*
 * handle the nuke command
 */
void
nuke() {
  // TODO: Add your code for the nuke command here...
  kill(0, SIGKILL);
}

