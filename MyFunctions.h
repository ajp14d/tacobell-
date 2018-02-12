// A collection of utility functions for the project
#ifndef _MYFUNCTIONS_H
#define _MYFUNCTIONS_H
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/stat.h>

// Big free
// Deallocates the memory in a 2D character array
void MemFunc(char** arr);

// PrintArgVector
// Prints out each argument on a separate line
void DisplayArgs(char** args);

// DeleteCharacter
// Deletes a series of characters from a dynamic character array
// The start and end iterations are inclusive
char* DelFunc(char* line, size_t begin, size_t end);

// FileExist
int ExistCheck(const char* file);

// IsExecutable
int ExecCheck(const char* file);

// DynStrCat
// This function frees dest and returns the new
// string with the format (dest, source)
char* LinkString(char* x, const char* y);

// DynStrPushBack
// This functions pushes a character onto the back of a dynamic
// string
char* BPushString(char* x, char y);

// DynStrPushFront
// This functions pushes a character onto the front of a dynamic
// string
char* FPushString(char* x, char y);

// RemoveLastDir
char* RmDir(char* p);

// IsCommand
int CmdCheck(char** args, int a);

// BuildPath
// Builds a path by resolving '.' '..' and '~'
char* PathMaker(char* s);

// PathEnvBuildPath
// Builds a path by first grabbing beginning of full path from the path env variable
char* PathFromEVar(char* s);

// ValidExternalCommand
int CmdVal(const char* s);

// CleanPathDirectory
// Removes the '/' at the end of a directory if there is one
char* PathClear(char* s);

// ReplaceSubStr
// Replaces the characters between iterators start and end (inclusive), with
// the string contained in source
char* CharRep(char* x, size_t begin, size_t end, const char* y);

// ContainsChar
// Checks if a string contains a character
int CharCheck(const char* s, char c);

// VecContainsStr
// Checks if an arg vector contains the string, and returns the first
// location of the string
// Returns -1 if the character does not exist in the vector
int StringCheck(char** argv, const char* s);

// ArraySize
// Computes the size (not including NULL) of an arg vector
int GetSize(char** argv);

// ArrayPushBack
// Pushes an argument onto the back of an argv
char** PBackArr(char** argv, const char* s);

// ArrayRemoveElement
// Removes an element from an argv
char** RemoveArr(char** argv, int a);

// ArgvToString
// Prints an argv to a string and returns the string
// The caller of this function is responsible for freeing memory
char* Convert(char** argv);

// ArgvCountSymbol
// Counts the number of times a string appears in an argument vector
int CountStr(char** argv, const char* s);

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
