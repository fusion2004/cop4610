// Lab 2 - Multithreaded Programming and Synchronization
// Part 2: News Conference Multithreaded Programming
// Mark Oleson & Alexis Jefferson
// COP4610 Operating Systems

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#include "part2.h"

/* Holds the ID of the reporter currently asking the questions,
   synchronized by the speech_mutex */
int question_asker = -1;

int started = 0;
int num_conference;

/* Conference room and has a limit to how many threads (reporters) can be
   allowed in at once */
sem_t conference_room_sem;

/* A lock on the speech to allow only one reporter to talk at one time */
pthread_mutex_t speech_mutex;

/* A lock to allow the speaker and reporter to take turns talking */
pthread_mutex_t floor_mutex;
pthread_cond_t  floor_cond;


int main(int argc, char *argv[]) {
  if(argc != 3) {
    printf("usage: %s <number of reporter threads> <capacity of conference room>\n", argv[0]);
    return 0;
  }
  else {
    int num_threads = atoi(argv[1]);
    num_conference  = atoi(argv[2]);

    char check_num_threads[strlen(argv[1]) + 1];
    char check_num_conference[strlen(argv[2]) + 1];

    sprintf(check_num_threads, "%d", num_threads);
    sprintf(check_num_conference, "%d", num_conference);

    if(strcmp(argv[1], check_num_threads)    != 0 ||
       strcmp(argv[2], check_num_conference) != 0 ||
       num_threads    <= 0 ||
       num_conference <= 0) {
      printf("Error: ARBRITRARY GARBAGE DETECTED. BOOTING KILLBOTS.\n");
      return -1;
    }

    /* Initializes the semaphore with the number of reporters allowed at one
       time. Also initializes the mutex locks and conditions.  */
    if(sem_init(&conference_room_sem, 0, num_conference) ||
       pthread_mutex_init(&speech_mutex, NULL) ||
       pthread_mutex_init(&floor_mutex, NULL) ||
       pthread_cond_init(&floor_cond, NULL)) {
      printf("Error: Could not initialize mutex/cond/sem.\n");
      return -1;
    }

    int i;
    pthread_t speaker;
    pthread_t reporters[num_threads];

    /* First, the floor is locked, to now allow any thread to start speaking
      until all threads are ready */
    pthread_mutex_lock(&floor_mutex);

    speaker = Speaker();
    if(speaker == 0) {
      printf("Error: Could not create Speaker thread.\n");
      return -1;
    }

    /* Now we wait on the condition, this is to allow the speaker to be created
       and then the speaker will signal and wait */
    pthread_cond_wait(&floor_cond, &floor_mutex);

    /* Now the speaker is done being created and is currently waiting. Setting
       var started to 1, to represent that we have started. */
    started = 1;
    pthread_mutex_unlock(&floor_mutex);

    /* Reporters are being created */
    for(i = 0; i < num_threads; i++) {
      reporters[i] = Reporter(i);
      if(reporters[i] == 0) {
        printf("Error: Could not create Reporter(%d) thread.\n", i);
        return -1;
      }
    }

    /* Main joins the Reporter threads and exits when they are finished */
    for(i = 0; i < num_threads; i++) {
      if(pthread_join(reporters[i], NULL)) {
        printf("Error: Could not join Reporter(%d) thread.\n", i);
        return -1;
      }
    }

  }
  return 0;
}

/* Creates the speaker thread. Returns 0 if the thread was not created properly,
   else returns the thread. */
int Speaker() {
  pthread_t thread;
  if(pthread_create(&thread, NULL, (void *)&SpeakerThread, NULL)) {
    return 0;
  }
  return thread;
}

/* Creates a reporter thread. Returns 0 if the thread was not created properly,
   else returns the thread. */
int Reporter(int id) {
  pthread_t thread;
  if(pthread_create(&thread, NULL, (void *)&ReporterThread, (void *)id)) {
    return 0;
  }
  return thread;
}

/* If not started (aka, started = 0), then signal on the condition. That will
   unlock the floor_mutex and allow another to aquire the lock.
   The reporter will aquire the lock.
   When it has started (aka started = 1), and the speaker will have to wait on
   the condition floor_cond to be signaled.
   This signal comes from the QuestionStart() function from a reporter. So the
   speaker starts answering the question. */
void AnswerStart() {
  if(!started) {
    pthread_cond_signal(&floor_cond);
  }
  pthread_cond_wait(&floor_cond, &floor_mutex);

  printf("Speaker starts to answer question for reporter %d.\n", question_asker);
}

/* After a speaker answers the question, he signals the floor_cond, which will
   awake the reporter thread who is waiting for his question to be done (to
   call QuestionDone()). */
void AnswerDone() {
  printf("Speaker is done with answer for reporter %d.\n", question_asker);

  pthread_cond_signal(&floor_cond);
//  pthread_mutex_unlock(&floor_mutex);
}

/* When a reporter enters the conference room, he first has to wait to see if
   the conference room has space. If there is space, he enters. If there isn't
   he waits until there is space. */
void EnterConferenceRoom(int id) {
  sem_wait(&conference_room_sem);
  printf("Reporter %d enters the conference room.\n", id);
}

/* The reporter leaves the conference room and tells the semaphore that there
   is a new empty slot. */
void LeaveConferenceRoom(int id) {
  printf("Reporter %d leaves the conference room.\n", id);
  sem_post(&conference_room_sem);
}

/* The reporter starts his question. He must first aquire the speech_mutex, so
   that only he can talk and no one else can interrupt him. He then signals the
   Speaker to answer the question. Afterwards, he waits for the Speaker. */
void QuestionStart(int id) {
  pthread_mutex_lock(&speech_mutex);

  printf("Reporter %d asks a question.\n", id);
  question_asker = id;

  pthread_mutex_lock(&floor_mutex);
  pthread_cond_signal(&floor_cond);
  pthread_cond_wait(&floor_cond, &floor_mutex);
}

/* Now the floor_mutex and speech_mutex locks are unlocked for the speaker and
   a new reporter can try to take the speech_mutex lock. */
void QuestionDone(int id) {
  pthread_mutex_unlock(&floor_mutex);

  printf("Reporter %d is satisfied.\n", id);
  question_asker = -1;

  pthread_mutex_unlock(&speech_mutex);
}

/* The function that the Speaker uses when its thread is created */
void SpeakerThread(void *args) {
  pthread_mutex_lock(&floor_mutex);
  while(1) {
    AnswerStart();
    AnswerDone();
  }
}

/* The function that the reporters use when their thread is created.
   The reporters are given an ID. The number of questions the reporter asks is
   based off their ID.
   The reporter then enters the conference room. Once inside the conference
   room, he asks his questions.
   The questions are decremented until there are no more questions. Then the
   reporter leaves the conference room. */
void ReporterThread(void *args) {
  int id = (int)args;
  int questions = id % 4 + 2;

  EnterConferenceRoom(id);

  while(questions > 0) {
    QuestionStart(id);
    QuestionDone(id);

    questions--;
  }

  LeaveConferenceRoom(id);
}
