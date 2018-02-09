// Header to outline all tasks related to parsing
#ifndef _PARSE_H
#define _PARSE_H

// ReadInput
// Grabs the input from the user and returns it as a dynamic character array
// The caller of this function holds the responsibility of freeing the memory
char* ReadInput();

// ParseInput
// Returns an array of c-strings (each argument in a different slot)
// All cells are dynamic--the caller of this function has the responsibility
// of freeing all memory
char** ParseInput(char* input);

// ParseWhitespace
// Returns the input string from the user with proper whitespace formating.
// Only one whitespace will be present between each argument with no leading
// or trailing whitespace.
char* ParseWhitespace(char* input);

// ParseArguments
// Converts the line input from the user into the array of command
// arguments.
// Dynamic memory allocated in this function. Caller of this function
// is responsibile for freeing the memory.
char** ParseArguments(char* input);

// ResolvePaths
// Resolves all paths in the command argument array
char** ResolvePaths(char** args);

// ExpandVariables
// Expands all environment variables in the command argument array
char** ExpandVariables(char** args);

#endif
