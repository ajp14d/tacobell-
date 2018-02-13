#include "pipe.h"


void OnePipe(char** argv1, char** argv2, int background, char* cmd)
{
	int status;
	int p1_to_p2[2];
	pipe(p1_to_p2);
	
	// fork1
	pid_t c1PID = fork();
	
	// parent of fork1
	if (c1PID > 0)
	{
		pid_t c2PID = fork();
		// parent of fork2
		if (c2PID > 0)
		{
			close(3);
			close(4);
			if (background != -1)
			{
				waitpid(c2PID, &status, WNOHANG);
				waitpid(c1PID, &status, WNOHANG);
				handleQueue(newPro(c1PID, c2PID, cmd));
			}
			else
			{
				waitpid(c2PID, &status, 0);
				waitpid(c1PID, &status, 0);
			}
		}
		// child of fork2
		else if(c2PID == 0)
		{
			close(0);
			dup(3);
			close(3);
			close(4);
			execvp(argv2[0], argv2);
			
			printf("Error executing in fork in OnePipe: \n");
			DisplayArgs(argv2);
		}
		else
		{
			printf("Error executing in fork in OnePipe: \n");
			DisplayArgs(argv2);
		}
		
	}
	// in child of fork1
	else if (c1PID == 0)
	{
		// close stdout
		close(1);
		dup(4);
		close(3);
		close(4);
		execvp(argv1[0], argv1);
		
		printf("Error executing in fork in OnePipe: \n");
		DisplayArgs(argv1);
	}
	else
	{
		printf("Error in fork1 of OnePipe()\n");
	}
}

void TwoPipe(char** argv1, char** argv2, char** argv3, int background, char* cmd)
{
	int status;
	int p1_to_p2[2];
	int p2_to_p3[2];
	
	pipe(p1_to_p2);
	pipe(p2_to_p3);
	
	pid_t c1PID = fork();
	
	// in parent of fork 1
	if (c1PID > 0)
	{
		pid_t c2PID = fork();
		//still in parent
		if (c2PID > 0)
		{
			pid_t c3PID = fork();
			// still in parent
			if (c3PID > 0)
			{
				close(3);
				close(4);
				close(5);
				close(6);
				
				waitpid(c3PID, &status, 0);
				waitpid(c2PID, &status, 0);
				waitpid(c1PID, &status, 0);
			}
			// in child of fork 3
			else if (c3PID == 0)
			{
				close(0);
				dup(5);
				close(3);
				close(4);
				close(5);
				close(6);
				
				execvp(argv3[0], argv3);
				
				printf("Error executing in fork of TwoPipe: \n");
				DisplayArgs(argv3);
			}
			else
			{
				printf("Error executing in fork in TwoPipe: \n");
				DisplayArgs(argv3);
			}
		}
		// child of fork 2
		else if (c2PID == 0)
		{
			close(0);
			dup(3);
			close(1);
			dup(6);
			close(3); 
			close(4); 
			close(5); 
			close(6);
			
			execvp(argv2[0], argv2);
			
			printf("Error executing in fork of TwoPipe: \n");
			DisplayArgs(argv2);
		}
		else
		{
			printf("Error executing in fork in TwoPipe: \n");
			DisplayArgs(argv2);
		}
	}
	// in child of fork 1
	else if (c1PID == 0)
	{
		close(1);
		dup(4);
		close(3);
		close(4);
		close(5);
		close(6);
		
		execvp(argv1[0], argv1);
		
		printf("Error executing in fork of TwoPipe: \n");
		DisplayArgs(argv1);
	}
	else
	{
		printf("Error executing in fork in TwoPipe: \n");
		DisplayArgs(argv1);
	}
}

void ThreePipe(char** argv1, char** argv2, char** argv3, char** argv4, int background, char* cmd)
{
	int status;
	int p1_to_p2[2];
	int p2_to_p3[2];
	int p3_to_p4[2];
	
	pipe(p1_to_p2);
	pipe(p2_to_p3);
	pipe(p3_to_p4);
	
	pid_t c1PID = fork();
	
	// inside parent
	if (c1PID > 0)
	{
		pid_t c2PID = fork();
		
		// still inside parent
		if (c2PID > 0)
		{
			pid_t c3PID = fork();
			
			// still inside parent
			if (c3PID > 0)
			{
				pid_t c4PID = fork();
				
				// still inside parent
				if (c4PID > 0)
				{
					close(3);
					close(4);
					close(5);
					close(6);
					close(7);
					close(8);
					
					waitpid(c4PID, &status, 0);
					waitpid(c3PID, &status, 0);
					waitpid(c2PID, &status, 0);
					waitpid(c1PID, &status, 0);
				}
				// inside child of fork 4
				else if (c4PID == 0)
				{
					close(0);
					dup(7);
					close(3);
					close(4);
					close(5);
					close(6);
					close(7);
					close(8);
					
					execvp(argv4[0], argv4);
					
					printf("Error executing in fork of ThreePipe: \n");
					DisplayArgs(argv4);
				}
				else
				{
					printf("Error executing in fork in ThreePipe: \n");
					DisplayArgs(argv4);
				}
			}
			// inside child of fork 3
			else if (c3PID == 0)
			{
				close(0);
				dup(5);
				close(1);
				dup(8);
				close(3);
				close(4);
				close(5);
				close(6);
				close(7);
				close(8);
				
				execvp(argv3[0], argv3);
				
				printf("Error executing in fork of ThreePipe: \n");
				DisplayArgs(argv3);
			}
			else
			{
				printf("Error executing in fork in ThreePipe: \n");
				DisplayArgs(argv3);
			}
		}
		// inside child of fork 2
		else if (c2PID == 0)
		{
			close(0);
			dup(3);
			close(1);
			dup(6);
			close(3);
			close(4);
			close(5);
			close(6);
			close(7);
			close(8);
			
			execvp(argv2[0], argv2);
			
			printf("Error executing in fork of ThreePipe: \n");
			DisplayArgs(argv2);
		}
		else
		{
			printf("Error executing in fork in ThreePipe: \n");
			DisplayArgs(argv2);
		}
	}
	// inside child of fork 1
	else if (c1PID == 0)
	{
		close(1);
		dup(4);
		close(3);
		close(4);
		close(5);
		close(6);
		close(7);
		close(8);
		
		execvp(argv1[0], argv1);
		
		printf("Error executing in fork of ThreePipe: \n");
		DisplayArgs(argv1);
	}
	else
	{
		printf("Error executing in fork in ThreePipe: \n");
		DisplayArgs(argv1);
	}
}
