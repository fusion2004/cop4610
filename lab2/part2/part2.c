// Lab 2 - Multithreaded Programming and Synchronization
// Part 2: News Conference Multithreaded Programming
// Mark Oleson & Alexis Jefferson
// COP4610 Operating Systems

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "part2.h"

int question_asker = -1;
int num_conference;

pthread_mutex_t speech_mutex;
pthread_mutex_t question_mutex;

pthread_mutex_t reporter_mutex;
pthread_cond_t  reporter_cond;
pthread_mutex_t speaker_mutex;
pthread_cond_t  speaker_cond;

int main(int argc, char *argv[]) {
  if(argc != 3) {
    printf("usage: usage goes here\n");
  }
  else {
    int num_threads = atoi(argv[1]);
    num_conference  = atoi(argv[2]);

    char check_num_threads[strlen(argv[1]) + 1];
    char check_num_conference[strlen(argv[2]) + 1];

    sprintf(check_num_threads, "%d", num_threads);
    sprintf(check_num_conference, "%d", num_conference);

    if(strcmp(argv[1], check_num_threads) != 0 ||
       strcmp(argv[2], check_num_conference) != 0) {
      printf("some errors\n");
      return -1;
    }

    if(pthread_mutex_init(&speech_mutex, NULL) &&
       pthread_mutex_init(&question_mutex, NULL) &&
       pthread_mutex_init(&reporter_mutex, NULL) &&
       pthread_cond_init(&reporter_cond, NULL) &&
       pthread_mutex_init(&speaker_mutex, NULL) &&
       pthread_cond_init(&speaker_cond, NULL)) {
      printf("more errors\n");
    }

    //pthread_t speaker;
    //pthread_t reporters[num_threads];

  }
  return 0;
}

void Speaker() {
}
void Reporter(int id) {
}
void AnswerStart() {
}
void AnswerDone() {
}
void EnterConferenceRoom() {
}
void LeaveConferenceRoom() {
}
void QuestionStart() {
}
void QuestionDone() {
}

void SpeakerThread(void *args) {
  while(1) {
    AnswerStart();
    AnswerDone();
  }
}
void ReporterThread(void *args) {
  int id = (int)args;
  int questions = id % 4 + 2;

  EnterConferenceRoom();

  while(questions > 0) {
    QuestionStart();
    QuestionDone();

    questions--;
  }

  LeaveConferenceRoom();
}
