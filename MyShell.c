#include "MyShell.h"

int main()
{
	RunShell();
	return 0;
}

void RunShell()
{
	int run = 1;
	char* str;
	char** argv;
	Init();
	while (run)
	{
		//KillZombies();
		checkQueue();
		PrintPrompt();
		str = GetInput();
		argv = ParseI(str);
		
		if ((argv[0] == NULL) || CheckForIOandPipeErrors(argv) || CheckForBackgroundErrors(argv))
		{
			// do nothing if empty arguments
		}
		else if (strcmp(argv[0], "exit") == 0)
		{
			MemFunc(argv);
			printf("Exiting Shell...\n");
			exitQueue();
			exit(0);
		}
		else if (strcmp(argv[0], "cd") == 0)
		{
			if (GetSize(argv) <= 2)
			{
				if (GetSize(argv) == 2)
					ChangeDirectory(argv[1]);
				else
					ChangeDirectory(getenv("HOME"));
			}
			else
			{
				printf("Too many arguments...\n");
			}
		}
		else if (strcmp(argv[0], "io") == 0)
		{
			if (GetSize(argv) > 1)
			{
				argv = RemoveArr(argv, 0);
				Limits(argv);
			}
		}
		else if (strcmp(argv[0], "etime") == 0)
		{
			printf("HELLO\n");
			if (GetSize(argv) > 1)
			{
				
				argv = RemoveArr(argv, 0);
				ETime(argv);
			}
		}
		else if (ExecCheck(argv[0]))
		{
			int background = StringCheck(argv, "&");
			int I_loc = StringCheck(argv, "<");
			int O_loc = StringCheck(argv, ">");
			int pipe_count = CountStr(argv, "|");
			if (I_loc != -1)
			{
				argv = ExecuteExternalWithInput(argv, I_loc, background);
			}
			else if (O_loc != -1)
			{
				argv = ExecuteExternalWithOutput(argv, O_loc, background);
			}
			else if (pipe_count > 0)
			{
				argv = ExecuteExternalWithPipe(argv, pipe_count, background);
			}
			else
			{
				char* cmd = Convert(argv);
				if (background != -1)
				{
					argv = RemoveArr(argv, background);
				}
				ExecuteExternal(argv, background, cmd);
				free(cmd);
			}
		}


		MemFunc(argv);
	}
}

void ETime(char** argv)
{
	int status;
	struct timeval timeofday;
	gettimeofday(&timeofday, NULL);
	double beginning_time=timeofday.tv_sec+(timeofday.tv_usec/1000000.0);

	pid_t childPID = fork();

	switch(checkZero(childPID)){
	  case 2:
	  {
		execv(argv[0], argv);
                printf("Trouble executing: \n");
                DisplayArgs(argv);
                exit(1);
		break;
	  }
	  case 1:
		waitpid(childPID, &status, 0);
		break;
	  case 0:
		printf("Fork failed in ETime\n");
		break;
	}


	gettimeofday(&timeofday, NULL);
	double finishing_time=timeofday.tv_sec+(timeofday.tv_usec/1000000.0);
	printf("Elapsed Time: %f\n", finishing_time-beginning_time);
}



