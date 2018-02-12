COP4610 - Project 1


Project Team Members
- Holly Culver
- Abby Perry
- Hunter Davis

Division of Labor
- Holly Culver: Background Processing
- Abby Perry: Piping
- Hunter Davis: Parsing

How to Compile:
- Using the Makefile 
- type make

How to clean: 
-make clean 

Tar File Contents:
README.txt
main.c
makefile

Completed using: linprog


Brief Description of Each File:
Parsing: grabs input from the user and extracts the command name, the arguments, and background execution indicators. Allocates memory for the parsing arguments and converts them to dynamic character arrays. 

Utility: Allocates and deallocates memory, checks in files exist, makes/deletes directories, creates/ changes/ builds paths, and prints arguments to screen

Shell: creates a background to test commands given from the user as input. Completes functions such as I/O redirection and executes the pipe function and does error checking for both of these. 

Pipe: passes information from one process to the next, allows for communicaiton between multiple function calls. Manages and calculates the number of commands separated by "|"

Known Bugs:


Unfinished Portions:


Special Considerations:
