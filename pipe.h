#ifndef _PIPE_H
#define _PIPE_H

#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<stdio.h>
#include "background.h"
#include "utility.h"

void OnePipe(char** argv1, char** argv2, int background, char* cmd);
void TwoPipe(char** argv1, char** argv2, char** argv3, int background, char* cmd);
void ThreePipe(char** argv1, char** argv2, char** argv3, char** argv4, int background, char* cmd);

#endif
