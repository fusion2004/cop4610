// Lab 2 - Multithreaded Programming and Synchronization
// Part 2: News Conference Multithreaded Programming
// Mark Oleson & Alexis Jefferson
// COP4610 Operating Systems

#ifndef PART2_H
#define PART2_H

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
