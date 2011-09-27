// Lab 2 - Multithreaded Programming and Synchronization
// Part 2: News Conference Multithreaded Programming
// Mark Oleson & Alexis Jefferson
// COP4610 Operating Systems

#ifndef PART2_H
#define PART2_H

int Speaker();
int Reporter(int id);
void AnswerStart(int id);
void AnswerDone(int id);
void EnterConferenceRoom(int id);
void LeaveConferenceRoom(int id);
void QuestionStart();
void QuestionDone();

void SpeakerThread(void *args);
void ReporterThread(void *args);

#endif
