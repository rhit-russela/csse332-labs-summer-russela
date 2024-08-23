#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char **argv)
{
  /* TODO: Insert your code here. */
  if(argc == 2){
    sleep(atoi(argv[1]));
  }else{
    printf("Did not pass second argument\n");
  }

  exit(0);
}
