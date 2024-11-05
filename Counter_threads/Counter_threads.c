// Counter_threads.c
#include <stdio.h>
#include <stdlib.h>

#include <stdio.h>
#include <stdlib.h>

// Standart header POSIX for use library pthread
#include <pthread.h>

#define SIZE 100000

#include <stdatomic.h>

_Atomic int counter_threads_in_CPU = 0;
//int counter_threads_in_CPU = 0;
// This function contains logic that should be executed as the body of a separate thread
void *thread_body(void *arg){
  counter_threads_in_CPU++;
  int value = *((int *)arg);
  printf("%d\n", value);
  return NULL;

}


int main(void)
{
    pthread_t threads[SIZE];
    int counter = 0;
    int result = 0;
    while(counter < SIZE)
    {
       result = pthread_create(&threads[counter], NULL, thread_body, &counter);
       if(result)
       {
          printf("Thread could not be created. Error number: %d\n", result);
          break;
       }
       counter++;
    }
    printf("Numbers threads create %d\n", counter);
    printf("Numbers threads caught in the core %d\n", counter_threads_in_CPU);
    printf("Hello world!\n");
    return 0;
}
