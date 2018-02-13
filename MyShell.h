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

void RunShell();

void ChangeDirectory(const char* dir);



void ExecuteExternal(char** argv, int background, char* cmd);

char** ExecuteExternalWithInput(char** argv, int I_loc, int background);

char** ExecuteExternalWithOutput(char** argv, int O_loc, int background);

char** ExecuteExternalWithPipe(char** argv, int pipe_count, int background);

// For file IO redirection
// dir=0 for output redirect
// dir=1 for input redirect
void IORedirect(char** argv, int dir, char* filename, int background, char* cmd);

int CheckForIOandPipeErrors(char** argv);

int CheckForBackgroundErrors(char** argv);

void PrintPrompt();

void Limits(char** argv);

void ETime(char** argv);

int checkZero(int tocheck);

void KillZombies();



#endif
