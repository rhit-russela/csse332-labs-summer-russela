/* Copyright 2016 Rose-Hulman
   But based on idea from http://cnds.eecs.jacobs-university.de/courses/caoslab-2007/
   */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include <stdbool.h>

void signal_handle(int ig){
  while(wait(NULL) != -1);
}

void runCommand(char **command, int arguments, bool background){
  int status = -1;
  int pid = fork();
  if(background){
    if(arguments == 0){
    if(pid == 0){ //Child
      if(execlp(command[0], command[0], NULL) < 0){
	exit(1);
      }
    exit(0);
    }else{ //Parent
      wait(&status);
      printf("Background Command is finished.\n");
    }
    }else{
      if(pid == 0){ //Child
	if(execlp(command[0], command[0], command[1], NULL) < 0){
	  exit(1);
	}
	exit(0);
      }else{ //Parent
	wait(&status);
	printf("Background Command is finished.\n");
      }
    }
  }
  if(arguments == 0){
    if(pid == 0){ //Child
      if(execlp(command[0], command[0], NULL) < 0){
	exit(1);
      }
    exit(0);
  }else{ //Parent
    wait(&status);
  }
}else{
  if(pid == 0){ //Child
    if(execlp(command[0], command[0], command[1], NULL) < 0){
      exit(1);
    }
    exit(0);
  }else{ //Parent
    wait(&status);
  }
 }
  
}


int main() {
  char command[82];
  char *parsed_command[2];
  //takes at most two input arguments
  // infinite loop but ^C quits

  signal(SIGCHLD, signal_handle);
  
  while (1) {
    printf("SHELL%% ");
    fgets(command, 82, stdin);
    command[strlen(command) - 1] = '\0';//remove the \n
    int len_1;
    for(len_1 = 0;command[len_1] != '\0';len_1++){
      if(command[len_1] == ' ')
        break;
    }
    parsed_command[0] = command;
    if(len_1 == strlen(command)){
      printf("Command is '%s' with no arguments\n", parsed_command[0]);
      parsed_command[1] = NULL;

      if(parsed_command[0][0] == 'B' && parsed_command[0][1] == 'G'){
	pid_t pid = fork();
	parsed_command[0] = parsed_command[0] + 2;
	if(pid == 0){
	  runCommand(parsed_command, 0, true);
	  exit(0);
	}
      }

      if(isdigit(parsed_command[0][0])){
	char *new_parsed[2];
	new_parsed[0] = parsed_command[0];
	char *a = parsed_command[0];
	for(int i = 0; i < atoi(a); i++){
	  new_parsed[0] = parsed_command[0] + 1;
	  runCommand(new_parsed, 0, false);
	}
      }

      runCommand(parsed_command, 0, false);
      
      // leave this here, do not change it
      if(!strcmp(parsed_command[0], "quit") ||
          !strcmp(parsed_command[0], "exit")) {
        exit(EXIT_SUCCESS);
      }
    }else{
      command[len_1] = '\0';
      parsed_command[1] = command + len_1 + 1;
      printf("Command is '%s' with argument '%s'\n", parsed_command[0], parsed_command[1]);

       if(parsed_command[0][0] == 'B' && parsed_command[0][1] == 'G'){
	pid_t pid = fork();
	parsed_command[0] = parsed_command[0] + 2;
	if(pid == 0){
	  runCommand(parsed_command, 1, true);
	  exit(0);
	}
      }

      if(isdigit(parsed_command[0][0])){
	char *new_parsed[2];
	new_parsed[0] = parsed_command[0];
	new_parsed[1] = parsed_command[1];
	char *a = parsed_command[0];
	for(int i = 0; i < atoi(a); i++){
	  new_parsed[0] = parsed_command[0] + 1;
	  runCommand(new_parsed, 1, false);
	}
      }

      runCommand(parsed_command, 1, false);
      
      // leave this here, do not change it
      if(!strcmp(parsed_command[0], "quit") ||
          !strcmp(parsed_command[0], "exit")) {
        exit(EXIT_SUCCESS);
      }
    }

  }
}
