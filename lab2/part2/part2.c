// Lab 2 - Multithreaded Programming and Synchronization
// Part 2: News Conference Multithreaded Programming
// Mark Oleson & Alexis Jefferson
// COP4610 Operating Systems

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "part2.h"

int main(int argc, char *argv[]) {
  return 0;
}

void Speaker();
void Reporter(int id);
void AnswerStart();
void AnswerDone();
void EnterConferenceRoom();
void LeaveConferenceRoom();
void QuestionStart();
void QuestionDone();

void SpeakerThread(void *args);
void ReporterThread(void *args);

#endif
