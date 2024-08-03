#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

// constants
#define NUM_FILES 5
#define NUM_THREADS 5
#define FILE_SIZE 10 * 1024

// function signatures, see the implementation at the end of the file
void transform_file(int, const char*, const char*);
void read_file_to_buffer(char*, const char*);

// the sum of all e characters in the all files
volatile int sum = 0;

// TODO: ADD LOCKING MECHANISMS HERE IF NEEDED
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;

// TODO: ADD YOUR THREADING STRUCTURES AND THREADING FUNCTIONS HERE

struct info{
const char *fileName;
const char *ofName;
int id;
};


void* threadCount(void* info){
  struct info* information = (struct info*) info;
  const char *ofname = information->ofName;
  int threadID = information->id;
  char buff[FILE_SIZE];

 printf("[Counting Thread %d] Working on file %s\n", threadID, ofname);

 pthread_mutex_lock(&lock2);
 read_file_to_buffer(buff, ofname);
    for(int j = 0; j < FILE_SIZE; j++) {
      if(buff[j] == 'e') {
        sum++;
      }
    }
 pthread_mutex_unlock(&lock2);
 return 0;
}


void* threadFunc(void* info){
  struct info* information = (struct info*) info;
  const char *file_name = information->fileName;
  const char *ofname = information->ofName;
  int threadID = information->id;

  struct timeval startt;

  pthread_t thread[1];

  struct info data;
  data.fileName = file_name;
  data.ofName = ofname;
  data.id = threadID;

  gettimeofday(&startt, NULL);
  transform_file(0, file_name, ofname);
  printf("[Thread %d] Working on file %s\n", threadID, file_name);
  //vprintf("[Thread DATA] Working on file %s to ofname: %s\n", data.fileName, data.ofName);
  pthread_create(&thread[0], NULL, threadCount, (void*)&data);
  pthread_join(thread[0], NULL);
  return 0;
}


int main(int argc, char **argv) {
  // YOU ARE NOT ALLOWED TO MAKE THESE ARRAYS GLOBAL VARIABLES, THEY MUST REMAIN
  // LOCAL VARIABLES TO THE MAIN THREAD
  const char *file_names[] = {"files/file1.dat", "files/file2.dat",
                              "files/file3.dat", "files/file4.dat",
                              "files/file5.dat"};

  const char *ofnames[] = {"files/output_file1.txt", "files/output_file2.txt",
                           "files/output_file3.txt", "files/output_file4.txt",
                           "files/output_file5.txt"};
  int i, j, seq_sum = 0;
  char buff[FILE_SIZE];
  struct timeval startt, stopt;
  suseconds_t time_cost;

  // Step 1: Do the stuff sequentially
  gettimeofday(&startt, NULL);
  for(i = 0; i < NUM_FILES; i++) {
    transform_file(0, file_names[i], ofnames[i]);
  }

  // Step 1: Count the stuff
  for(i = 0; i < NUM_FILES; i++) {
    read_file_to_buffer(buff, ofnames[i]);
    for(j = 0; j < FILE_SIZE; j++) {
      if(buff[j] == 'e') {
        seq_sum++;
      }
    }
  }
  printf("Sequential sum approach computed %d 'e' chars\n", seq_sum);
  gettimeofday(&stopt, NULL);
  time_cost = stopt.tv_usec - startt.tv_usec;
  printf("Sequential approach finished in %ld usecs.\n", time_cost);

  /////////////////////////////////////////////////////////////////////////////
  // DO NOT CHANGE THE CODE ABOVE THIS
  /////////////////////////////////////////////////////////////////////////////

  gettimeofday(&startt, NULL);

  // TODO: ADD YOUR PARALLEL CODE HERE

  pthread_t threads[5];
  struct info information;

  for(int i = 0; i < 5; i++){
  information.fileName = file_names[i];
  information.ofName = ofnames[i];
  information.id = i;
  printf("Thread %d is being created\n", i);
  pthread_create(&threads[i], NULL, threadFunc, (void*)&information);
  }


  for(int k = 0; k < 5; k++){
   pthread_join(threads[k], NULL);
  }
  // END OF YOUR ADDED CODE

  printf("Parallel sum approach computed %d 'e' chars\n", sum);
  gettimeofday(&stopt, NULL);
  time_cost = stopt.tv_usec - startt.tv_usec;
  printf("Parallel approach finished in %ld usecs.\n", time_cost);

  exit(EXIT_SUCCESS);
}


////////////////////////////////////////////////////////////////////////////////
// FEEL FREE TO IGNORE THE CODE BELOW HERE
////////////////////////////////////////////////////////////////////////////////

/**
 * Transform a file from hex strings into a set of ASCII characters.
 *
 * @tid     The id of the thread making this transformation.
 * @fname   The name of the file to read the hex characters from.
 * @ofname  The name of the output file to write to.
 *
 * This function will read the content of fname as a set hex strings and will
 * transform those into ASCII characters. It will then write those ASCII
 * characters into another file of name ofname.
 *
 * @warning Do not edit this function, all generated files will be cleaned up
 * when doing a `make clean`.
 */
void transform_file(int tid, const char *fname, const char *ofname) {
  FILE *fp, *wfp;
  char hexstr[5]; // each hex string is 0xab and null char
  char c;

  fp = fopen(fname, "r");
  if(!fp) {
    // kill everything upon failing to read the file
    fprintf(stderr, "Could not open file %s correcty\n", fname);
    exit(EXIT_FAILURE);
  }

  wfp = fopen(ofname, "w");
  if(!wfp) {
    // kill everything upon failing to open the output file
    fprintf(stderr, "Could not open file %s correcty\n", ofname);
    exit(EXIT_FAILURE);
  }

  while(fgets(hexstr, 5, fp)) {
    // skip over a new line if it's there
    if(!strcmp(hexstr, "\n"))
      continue;
    c = (char)strtoul(hexstr, 0, 16);
    fputc(c, wfp);
  }

  fclose(fp);
  fclose(wfp);
}

/**
 * Read a file into a character buffer.
 *
 * @buffer    The buffer to read into.
 * @fname     The name of the file to read from.
 *
 * This function will read the content of the file fname and then store them
 * into the buffer buffer. It will read at most FILE_SIZE - 1 bytes from the
 * file.
 */
void read_file_to_buffer(char *buffer, const char *fname) {
  FILE *fp;

  fp = fopen(fname, "r");
  if(!fp) {
    fprintf(stderr, "Cannot open file %s, serious problems have happened!",
            fname);
    exit(EXIT_FAILURE);
  }
  fgets(buffer, FILE_SIZE, fp);
  fclose(fp);
}
