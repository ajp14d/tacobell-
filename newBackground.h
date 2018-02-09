#ifndef _BACKGROUND_H
#define _BACKGROUND_H

#include<stdlib.h>      // for NULL decleration
#include<string.h>      // for string functionality (strcpy)
#include<stdio.h>       // for printing out processes
#include<sys/wait.h>

struct beginPro
{
        // pid for first command
        int pid_1;
        // if there is redirection, this contains
        // the pid for the second command
        // if not, this assumes the value -1
        int pid_2;
        // the cmd used to spawn the process
        char* cmd;
};

// global process queue
#define PQUEUE_SIZE 256
struct beginPro* process_queue[PQUEUE_SIZE];

void Init();

void handleQueue(struct beginPro* p);

void checkQueue();

void exitQueue();

void remPro(int index);

struct beginPro* newPro(int pid_1, int pid_2,
        char* cmd);

void spacePro(struct beginPro* p);

int checkPro(struct beginPro* p);

void displayProStart(const struct beginPro* p, int pos);

void displayProDone(const struct beginPro* p, int pos);

#endif
