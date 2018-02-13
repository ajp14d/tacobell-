all:  main.o MyParse.o MyFunctions.o  MyShell.o MyProcesses.o pipe.o
	gcc -Wall -pedantic -std=c99 -g -o MyShell.x  main.o MyParse.o MyFunctions.o  MyShell.o MyProcesses.o pipe.o

main.o: main.c
	gcc -Wall -pedantic -std=c99 -g -c main.c

MyParse.o: MyParse.c MyParse.h
	gcc -Wall -pedantic -std=c99 -g -c MyParse.c

MyFunctions.o: MyFunctions.c MyFunctions.h
	gcc -Wall -pedantic -std=c99 -g -c MyFunctions.c

MyShell.o: MyShell.c MyShell.h
	gcc -Wall -pedantic -std=c99 -g -c MyShell.c

MyProcesses.o: MyProcesses.c MyProcesses.h
	gcc -Wall -pedantic -std=c99 -g -c MyProcesses.c
	
pipe.o: pipe.c pipe.h
	gcc -Wall -pedantic -std=c99 -g -c pipe.c

clean:
	rm *.o MyShell.x 
