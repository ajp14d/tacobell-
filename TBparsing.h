#ifndef _TBPARSING_H
#define _TBPARSING_H

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
