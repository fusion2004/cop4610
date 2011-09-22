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

int shared_variable = 0;

int main(int argc, char *argv[]) {
  if(argc != 2) {
    printUsage(argv[0]);
  } else {
    int num_threads = atoi(argv[1]);
    char check_it[strlen(argv[1]) + 1];

    sprintf(check_it, "%d", num_threads);

    if(strcmp(argv[1], check_it) != 0) {
      printInputError();
    } else {
      int thread, i;
      pthread_t th_id[num_threads+1];
      for(i = 1; i <= num_threads; i++) {
        thread = pthread_create(&th_id[i], NULL, (void *)&SimpleThread, (void *)i);
      }
      for(i = 1; i <= num_threads; i++) {
        pthread_join(th_id[i], NULL);
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
  //printf("Thread started with value: %d\n", which);
  int num, val;

  for(num = 0; num < 20; num++) {
    if(random() > RAND_MAX / 2) {
      usleep(10);
    }

    val = shared_variable;
    printf("*** thread %d sees value %d\n", which, val);
    shared_variable = val + 1;
  }

  val = shared_variable;
  printf("Thread %d sees final value %d\n", which, val);

  //return 0;
}
