#ifndef _MYPROCESSES_H
#define _MYPROCESSES_H

#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/time.h>
#include<stdbool.h>
//#include "pipe.h"

struct beginPro
{
        // pid for first command
        int firstpid;
        // if there is redirection, this contains
        // the pid for the second command
        // if not, this assumes the value -1
        int secondpid;
        // the cmd used to spawn the process
        char* thing;
};

// global process queue
#define THEQUEUE_SIZE 256
struct beginPro* thequeue[THEQUEUE_SIZE];

void Init();

void handleQueue(struct beginPro* loc);

void checkQueue();

void exitQueue();

void remPro(int ind);

struct beginPro* newPro(int firstpid, int secondpid, char* thing);

void spacePro(struct beginPro* pro);

int checkPro(struct beginPro* pro);

void displayProStart(const struct beginPro* pro, int loc);

void displayProDone(const struct beginPro* pro, int loc);

bool checkNull(const struct beginPro* tocheck);

#endif
