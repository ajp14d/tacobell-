all:  MyParse.o MyFunctions.o  MyShell.o MyProcesses.o 
	gcc -Wall -pedantic -std=c99 -g -o MyShell.x  MyParse.o MyFunctions.o  MyShell.o MyProcesses.o 

MyParse.o: MyParse.c MyParse.h
	gcc -Wall -pedantic -std=c99 -g -c MyParse.c

MyFunctions.o: MyFunctions.c MyFunctions.h
	gcc -Wall -pedantic -std=c99 -g -c MyFunctions.c

MyShell.o: MyShell.c MyShell.h
	gcc -Wall -pedantic -std=c99 -g -c MyShell.c

MyProcesses.o: MyProcesses.c MyProcesses.h
	gcc -Wall -pedantic -std=c99 -g -c MyProcesses.c

clean:
	rm *.o MyShell.x 
