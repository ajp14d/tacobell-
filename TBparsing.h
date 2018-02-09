// Header to outline all tasks related to parsing
#ifndef _PARSE_H
#define _PARSE_H

// Takes the input from the user and returns it as a dynamic character array
// Whoever calls this function must also free the memory
char* GetInput();

// Returns an array of c-strings (with each argument in a different slot)
// All cells are dynamic, Whoever calls this function must also free the memory
char** ParseI(char* input);

// Returns the input string from the user with proper whitespace formating.
// Only one whitespace will be present between each argument with no leading or trailing whitespace.
char* PWhitespace(char* input);

// Converts the line input from the user into the array of command arguments.
// Dynamic memory allocated in this function. Whoever calls this function must also free the memory
char** PArguments(char* input);

// Resolves all paths in the command argument array
char** PathResolve(char** args);

// Expands all environment variables in the command argument array
char** Expand(char** args);

#endif
