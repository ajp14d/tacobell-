#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<stdio.h>
#include "MyProcesses.h"
#include "MyFunctions.h"

void OnePipe(char** argv1, char** argv2, int background, char* cmd);
void TwoPipe(char** argv1, char** argv2, char** argv3, int background, char* cmd);
void ThreePipe(char** argv1, char** argv2, char** argv3, char** argv4, int background, char* cmd);

#ifndef _MYPARSE_H
#define _MYPARSE_H

// Takes the input from the user and returns it as a dynamic character array
// Whoever calls this function must also free the memory
char* GetInput();

// Resolves all paths in the command argument array
char** PathResolve(char** args);

// Returns the input string from the user with the proper whitespace formating.
// Only one whitespace will be between each argument with no leading or trailing whitespace.
char* PWhitespace(char* input);

// Returns an array of c-strings (with each argument in a different slot)
// All cells are dynamic, Whoever calls this function must also free the memory
char** ParseI(char* input);

// Expands all environment variables in the command argument array
char** Expand(char** args);

// Converts the line input from the user into the array of command arguments.
// Dynamic memory is allocated in this function. Whoever calls this function must also free the memory
char** PArguments(char* input);

#endif




