// A collection of utility functions for the project
#ifndef _UTILITY_H
#define _UTILITY_H
#include<stdlib.h>
#include<sys/stat.h>

// Big free
// Deallocates the memory in a 2D character array
void BigFree(char** array);

// PrintArgVector
// Prints out each argument on a separate line
void PrintArgVector(char** args);

// DeleteCharacter
// Deletes a series of characters from a dynamic character array
// The start and end iterations are inclusive
char* DeleteCharacter(char* line, size_t start, size_t end);

// FileExist
int FileExist(const char* filename);

// IsExecutable
int IsExecutable(const char* filename);

// DynStrCat
// This function frees dest and returns the new
// string with the format (dest, source)
char* DynStrCat(char* dest, const char* source);

// DynStrPushBack
// This functions pushes a character onto the back of a dynamic
// string
char* DynStrPushBack(char* dest, char source);

// DynStrPushFront
// This functions pushes a character onto the front of a dynamic
// string
char* DynStrPushFront(char* dest, char source);

// RemoveLastDir
char* RemoveLastDir(char* path);

// IsCommand
int IsCommand(char** args, int i);

// BuildPath
// Builds a path by resolving '.' '..' and '~'
char* BuildPath(char* str);

// PathEnvBuildPath
// Builds a path by first grabbing beginning of full path from the path env variable
char* PathEnvBuildPath(char* str);

// ValidExternalCommand
int ValidExternalCommand(const char* str);

// CleanPathDirectory
// Removes the '/' at the end of a directory if there is one
char* CleanPathDirectory(char* str);

// ReplaceSubStr
// Replaces the characters between iterators start and end (inclusive), with
// the string contained in source
char* ReplaceSubStr(char* dest, size_t start, size_t end, const char* source);

// ContainsChar
// Checks if a string contains a character
int ContainsChar(const char* str, char c);

// VecContainsStr
// Checks if an arg vector contains the string, and returns the first
// location of the string
// Returns -1 if the character does not exist in the vector
int VecContainsStr(char** argv, const char* str);

// ArraySize
// Computes the size (not including NULL) of an arg vector
int ArraySize(char** argv);

// ArrayPushBack
// Pushes an argument onto the back of an argv
char** ArrayPushBack(char** argv, const char* str);

// ArrayRemoveElement
// Removes an element from an argv
char** ArrayRemoveElement(char** argv, int index);

// ArgvToString
// Prints an argv to a string and returns the string
// The caller of this function is responsible for freeing memory
char* ArgvToString(char** argv);

// ArgvCountSymbol
// Counts the number of times a string appears in an argument vector
int ArgvCountSymbol(char** argv, const char* str);

#endif
