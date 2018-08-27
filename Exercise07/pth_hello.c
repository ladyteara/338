/* Pthreads hello from Pacheco, Ch4 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* Global variable: accessible to all threads */
int thr_count;

void* Hello(void* rank);  //Thread function

int main(int argc, char* argv[]) 
{
  long thr;  //Use long in case of 64 bit system
  pthread_t* thr_handles;

  //Get number of threads from cmd line
  thr_count = strtol(argv[1], NULL, 10);
  thr_handles = malloc(thr_count * sizeof(pthread_t));
  for (thr = 0; thr < thr_count; thr++) 
  {
    pthread_create(&thr_handles[thr], NULL, Hello, (void*) thr);
  }
  
  printf("Hello from the main thread!\n");
  
  for (thr = 0; thr < thr_count; thr++) 
  {
    pthread_join(thr_handles[thr], NULL);
  }
  
  free(thr_handles);
  
  return 0;
} //main


void* Hello(void* rank) 
{
  long my_rank = (long) rank;  //long for 64 bit system
  printf("Hello from thread %ld of %d\n", my_rank, thr_count);
  return NULL;
} //Hello
