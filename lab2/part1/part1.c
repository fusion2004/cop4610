// Lab 2 - Multithreaded Programming and Synchronization
// Part 1: Simple Multithreaded Programming
// Mark Oleson & Alexis Jefferson
// COP4610 Operating Systems

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#include "part1.h"

// The magical shared variable all the threads want to modify.
int shared_variable = 0;

// Some beautiful pthread constructs to stop the threads from BREAKING
// the wonderful shared variable.
pthread_mutex_t   shavar_mutex;
pthread_barrier_t barr;

int main(int argc, char *argv[]) {
  // If there are more or less than 2 arguments, we assume the user has typed
  // something incorrectly, and print out the Usage.
  if(argc != 2) {
    printUsage(argv[0]);
  }

  // If we have 2 args, parse the 2nd arg into an integer to define how
  // many threads to spawn. And then... we spawn 'em.
  else {
    int num_threads = atoi(argv[1]);

    // Now we parse the int BACK into a char* so we can double-check it
    // with the original arg. There may be an easier way of doing this.
    char check_it[strlen(argv[1]) + 1];
    sprintf(check_it, "%d", num_threads);

    // If it doesn't match, we'll print out an error and exit the program.
    if(strcmp(argv[1], check_it) != 0) {
      printInputError();
      return -1;
    }

    // If it does match, we'll continue and start spawning threads.
    else {
      int i;
      pthread_t th_id[num_threads+1];

#ifdef PTHREAD_SYNC
      // Initialize the barrier used to sync all the threads before
      // printing out the final value of the shared variable.
      if(pthread_barrier_init(&barr, NULL, num_threads)) {
        printf("Error: Could not create a barrier\n");
        return -1;
      }

      // Initialize the mutex used to lock and unlock the shared variable.
      if(pthread_mutex_init(&shavar_mutex, NULL)) {
        printf("Error: Unable to initialize a mutex\n");
        return -1;
      }
#endif

      // Spawn the threads.
      for(i = 1; i <= num_threads; i++) {
        if(pthread_create(&th_id[i], NULL, (void *)&SimpleThread, (void *)i)) {
          printf("Error: Could not create thread %d\n", i);
          return -1;
        }
      }

      // Join the main thread to all of the threads we just spawned.
      for(i = 1; i <= num_threads; i++) {
        if(pthread_join(th_id[i], NULL)) {
          printf("Error: Could not join thread %d\n", i);
          return -1;
        }
      }
    }
  }
  return 0;
}

void printUsage(char *application) {
  printf("usage: %s <# of threads to run>\n", application);
  printf("The number of threads to run must be an integer.\n");
}

void printInputError() {
  printf("Error: Incorrect input.\n");
}

void SimpleThread(void *args) {
  int which = (int)args;
  int num, val;

  for(num = 0; num < 20; num++) {
    if(random() > RAND_MAX / 2) {
      usleep(10);
    }

#ifdef PTHREAD_SYNC
    pthread_mutex_lock(&shavar_mutex);
#endif

    val = shared_variable;
    printf("*** thread %d sees value %d\n", which, val);
    shared_variable = val + 1;

#ifdef PTHREAD_SYNC
    pthread_mutex_unlock(&shavar_mutex);
#endif

  }

#ifdef PTHREAD_SYNC
  int rc = pthread_barrier_wait(&barr);
  if(rc != 0 && rc != PTHREAD_BARRIER_SERIAL_THREAD) {
    printf("Error: Could not wait on barrier\n");
    exit(-1);
  }
#endif
  val = shared_variable;
  printf("Thread %d sees final value %d\n", which, val);

  //return 0;
}
