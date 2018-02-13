#ifndef _MYSHELL_H
#define _MYSHELL_H
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/time.h>
#include "MyFunctions.h"
#include "MyParse.h"
#include "MyProcesses.h"
#include "pipe.h"

// For each function in here
// int background is a flag to run the process in the background (-1) for foreground
// any value >= 0 for background
// char* cmd is the argv formatted to be pushed into the process queue

void startMyShell();

void dirChange(const char* dir);

void theExtern(char** argv, int background, char* cmd);

char** externIn(char** argv, int I_loc, int background);

char** externOut(char** argv, int O_loc, int background);

char** externPipe(char** argv, int pipe_count, int background);

// For file IO redirection
// dir=0 for output redirect
// dir=1 for input redirect
void handleIO(char** argv, int dir, char* filename, int background, char* cmd);

int errorsPipeIO(char** argv);

int errorsBackground(char** argv);

void displayPrompt();

void ioCmd(char** argv);

void etimeCmd(char** argv);

int checkZero(int tocheck);

void KillZombies();



#endif
