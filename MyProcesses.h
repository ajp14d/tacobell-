#ifndef _MYPROCESSES_H
#define _MYPROCESSES_H

#include<stdlib.h>      // for NULL decleration
#include<string.h>      // for string functionality (strcpy)
#include<stdio.h>       // for printing out processes
#include<sys/wait.h>
#include<stdbool.h>

struct beginPro
{
        // pid for first command
        int firstpid;
        // if there is redirection, this contains
        // the pid for the second command
        // if not, this assumes the value -1
        int secondpid;
        // the cmd used to spawn the process
        char* do;
};

// global process queue
#define THEQUEUE_SIZE 256
struct beginPro* thequeue[THEQUEUE_SIZE];

void Init();

void handleQueue(struct beginPro* loc);

void checkQueue();

void exitQueue();

void remPro(int ind);

struct beginPro* newPro(int firstpid, int secondpid, char* do);

void spacePro(struct beginPro* pro);

int checkPro(struct beginPro* pro);

void displayProStart(const struct beginPro* pro, int loc);

void displayProDone(const struct beginPro* pro, int loc);

bool checkNull(const struct beginPro* tocheck)

#endif
