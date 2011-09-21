// Lab 2 - Multithreaded Programming and Synchronization
// Part 1: Simple Multithreaded Programming
// Mark Oleson & Alexis Jefferson
// COP4610 Operating Systems

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "part1.h"

int shared_variable = 0;

int main(int argc, char *argv[]) {
  if(argc != 2) {
    printUsage();
  }
}

void printUsage() {
  printf("usage: part1 <# of threads to run>\n");
//  printf("  The number of threads to run must be an integer.");
}

void SimpleThread(int which) {
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
}
