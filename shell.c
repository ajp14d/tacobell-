#include "shell.h"

void RunShell()
{
	int running = 1;
	char* str;
	char** argv;
	InitBackgroundProcessing();
	while (running)
	{
		CheckProcessQueue();
		PrintPrompt();
		str = ReadInput();
		argv = ParseInput(str);
		
		if (argv[0] == NULL)
		{
			// do nothing if empty arguments
		}
		else if (CheckForIOandPipeErrors(argv))
		{
			// do nothing if IO or pipe error
		}
		else if (CheckForBackgroundErrors(argv))
		{
			// do nothing if background processing format error
		}
		else if (strcmp(argv[0], "exit") == 0)
		{
			BigFree(argv);
			printf("Exiting Shell...\n");
			OnExitProcessQueueWait();
			exit(0);
		}
		else if (strcmp(argv[0], "cd") == 0)
		{
			if (ArraySize(argv) <= 2)
			{
				if (ArraySize(argv) == 2)
					ChangeDirectory(argv[1]);
				else
					ChangeDirectory(getenv("HOME"));
			}
			else
			{
				printf("Too many arguments...\n");
			}
		}
		else if (strcmp(argv[0], "limits") == 0)
		{
			if (ArraySize(argv) > 1)
			{
				argv = ArrayRemoveElement(argv, 0);
				Limits(argv);
			}
		}
		else if (strcmp(argv[0], "etime") == 0)
		{
			if (ArraySize(argv) > 1)
			{
				argv = ArrayRemoveElement(argv, 0);
				ETime(argv);
			}
		}
		else if (IsExecutable(argv[0]))
		{
			int background = VecContainsStr(argv, "&");
			int I_loc = VecContainsStr(argv, "<");
			int O_loc = VecContainsStr(argv, ">");
			int pipe_count = ArgvCountSymbol(argv, "|");
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
				char* cmd = ArgvToString(argv);
				if (background != -1)
				{
					argv = ArrayRemoveElement(argv, background);
				}
				ExecuteExternal(argv, background, cmd);
				free(cmd);
			}
		}
		BigFree(argv);
	}
}

void ChangeDirectory(const char* dir)
{
	int ret = chdir(dir);
	if (ret == 0)
	{
		setenv("PWD", dir, 1);
	}
	else
	{
		printf("No such directory\n");
	}
}

void ExecuteExternal(char** argv, int background, char* cmd)
{
	int status;
	pid_t pid = fork();
	if (pid == -1)
	{
		printf("Error on fork\n");
		exit(1);
	}
	else if (pid == 0)
	{
		execv(argv[0], argv);
		printf("Trouble executing: \n");
		PrintArgVector(argv);
		exit(1);
	}
	else
	{
		if (background != -1)
		{
			waitpid(pid, &status, WNOHANG);
			QueueProcess(CreateProcessEntry(pid, -1, cmd));
			//kill(pid, SIGSTOP);
			//Need to implement a pause here for things like text editors
		}
		else
			waitpid(pid, &status, 0);
	}
}

char** ExecuteExternalWithInput(char** argv, int I_loc, int background)
{
	char* filename = (char*)calloc(strlen(argv[I_loc+1])+1, sizeof(char));
	strcpy(filename, argv[I_loc+1]);

	argv = ArrayRemoveElement(argv, I_loc);
	argv = ArrayRemoveElement(argv, I_loc);
				
	// update background iterator
	background = VecContainsStr(argv, "&");
	char* cmd = ArgvToString(argv);
	if (background != -1)
	{
		argv = ArrayRemoveElement(argv, background);
	}
	IORedirect(argv, 1, filename, background, cmd);
	free(filename);
	free(cmd);
	
	return argv;
}

char** ExecuteExternalWithOutput(char** argv, int O_loc, int background)
{
	char* filename = (char*)calloc(strlen(argv[O_loc+1])+1, sizeof(char));
	strcpy(filename, argv[O_loc+1]);

	argv = ArrayRemoveElement(argv, O_loc);
	argv = ArrayRemoveElement(argv, O_loc);
			
	// update background iterator
	background = VecContainsStr(argv, "&");
	char* cmd = ArgvToString(argv);
	if (background != -1)
	{
		argv = ArrayRemoveElement(argv, background);
	}
	IORedirect(argv, 0, filename, background, cmd);
	free(filename);
	free(cmd);
	
	return argv;
}

char** ExecuteExternalWithPipe(char** argv, int pipe_count, int background)
{
	char* cmd = ArgvToString(argv);
	if (background != -1)
	{
		argv = ArrayRemoveElement(argv, background);
	}
	if (pipe_count == 1)
	{
		char** argv1 = (char**)calloc(1, sizeof(char*));
		char** argv2 = (char**)calloc(1, sizeof(char*));
		argv1[0] = NULL;
		argv2[0] = NULL;
		int it = 0;
		while(strcmp(argv[it], "|") != 0)
		{
			argv1 = ArrayPushBack(argv1, argv[it]);
			++it;
		}
		++it;
		while(argv[it] != NULL)
		{
			argv2 = ArrayPushBack(argv2, argv[it]);
			++it;
		}
		OnePipe(argv1, argv2, background, cmd);
		BigFree(argv1);
		BigFree(argv2);
	}
	else if (pipe_count == 2)
	{
		char** argv1 = (char**)calloc(1, sizeof(char*));
		char** argv2 = (char**)calloc(1, sizeof(char*));
		char** argv3 = (char**)calloc(1, sizeof(char*));
		argv1[0] = NULL;
		argv2[0] = NULL;
		argv3[0] = NULL;
		int it = 0;
		while(strcmp(argv[it], "|") != 0)
		{
			argv1 = ArrayPushBack(argv1, argv[it]);
			++it;
		}
		++it;
		while(strcmp(argv[it], "|") != 0)
		{
			argv2 = ArrayPushBack(argv2, argv[it]);
			++it;
		}
		++it;
		while(argv[it] != NULL)
		{
			argv3 = ArrayPushBack(argv3, argv[it]);
			++it;
		}
		TwoPipe(argv1, argv2, argv3, background, cmd);
		BigFree(argv1);
		BigFree(argv2);
		BigFree(argv3);
	}
	else if (pipe_count == 3)
	{
		char** argv1 = (char**)calloc(1, sizeof(char*));
		char** argv2 = (char**)calloc(1, sizeof(char*));
		char** argv3 = (char**)calloc(1, sizeof(char*));
		char** argv4 = (char**)calloc(1, sizeof(char*));
		argv1[0] = NULL;
		argv2[0] = NULL;
		argv3[0] = NULL;
		argv4[0] = NULL;
		int it = 0;
		while(strcmp(argv[it], "|") != 0)
		{
			argv1 = ArrayPushBack(argv1, argv[it]);
			++it;
		}
		++it;
		while(strcmp(argv[it], "|") != 0)
		{
			argv2 = ArrayPushBack(argv2, argv[it]);
			++it;
		}
		++it;
		while(strcmp(argv[it], "|") != 0)
		{
			argv3 = ArrayPushBack(argv3, argv[it]);
			++it;
		}
		++it;
		while(argv[it] != NULL)
		{
			argv4 = ArrayPushBack(argv4, argv[it]);
			++it;
		}
		ThreePipe(argv1, argv2, argv3, argv4, background, cmd);
		BigFree(argv1);
		BigFree(argv2);
		BigFree(argv3);
		BigFree(argv4);
	}
	free(cmd);
	return argv;
}

void IORedirect(char** argv, int dir, char* filename, int background, char* cmd)
{
	// output redirection
	if (dir == 0)
	{
		int status;
		int fd = open(filename, O_CREAT|O_WRONLY|O_TRUNC, 0777);
		if (fd == -1)
		{
			printf("Error. Cannot open filename: %s\n", filename);
			exit(1);
		}
		pid_t pid = fork();

		if (pid == 0)
		{
			close(1);
			dup(fd);
			close(fd);
			execv(argv[0], argv);
			printf("Unknown command in IORedirect\n");
			exit(1);
		}
		else if (pid > 0)
		{
			if (background != -1)
			{
				waitpid(pid, &status, WNOHANG);
				QueueProcess(CreateProcessEntry(pid, -1, cmd));
			}
			else
			{
				waitpid(pid, &status, 0);
			}
			close(fd);
		}
		else
		{
			printf("Fork failed in IORedirect\n");
			exit(1);
		}
	}
	else if (dir == 1)
	{
		int status;
		int fd = open(filename, O_RDONLY);
		if (fd == -1)
		{
			printf("Error, cannot access file in IORedirect!\n");
			exit(1);
		}

		pid_t pid = fork();
		
		if (pid == 0)
		{
			close(0);
			dup(fd);
			close(fd);
			execv(argv[0], argv);

			printf("Unknown command in IORedirect\n");
			exit(1);
		}
		else if (pid > 0)
		{
			if (background != -1)
			{
				waitpid(pid, &status, WNOHANG);
				QueueProcess(CreateProcessEntry(pid, -1, cmd));
			}
			else
			{
				waitpid(pid, &status, 0);
			}
			close(fd);
		}
		else
		{
			printf("Fork() failed in IORedirect\n");
			exit(1);
		}
	}
}

int CheckForIOandPipeErrors(char** argv)
{
	if (ArraySize(argv) == 1)
	{
		if ((VecContainsStr(argv, "<") != -1) ||
			(VecContainsStr(argv, ">") != -1) ||
			(VecContainsStr(argv, "|") != -1))
		{
			printf("Requires more than 1 argument\n");
			return 1;
		}
	}
	else
	{
		if ((VecContainsStr(argv, "<") == 0) ||
			(VecContainsStr(argv, ">") == 0) ||
			(VecContainsStr(argv, "|") == 0))
		{
			printf("Error, IO redirection or pipe cannot appear at beginning of statement\n");
			return 1;
		}
		else if ((VecContainsStr(argv, "<") == ArraySize(argv)-1) ||
				 (VecContainsStr(argv, ">") == ArraySize(argv)-1) ||
				 (VecContainsStr(argv, "|") == ArraySize(argv)-1))
		{
			printf("Error, IO redirection or pipe cannot appear at end of statement\n");
			return 1;
		}
	}
	return 0;
}

int CheckForBackgroundErrors(char** argv)
{
	int I_loc = VecContainsStr(argv, "<");
	int O_loc = VecContainsStr(argv, ">");
	int P_loc = VecContainsStr(argv, "|");
	int B_loc = VecContainsStr(argv, "&");
	
	if (B_loc == -1)
	{
		return 0;
	}
	
	// if not < > or |, no error
	if ((I_loc == -1) &&
		(O_loc == -1) &&
		(P_loc == -1))
	{
		return 0;
	}
	else
	{
		if (I_loc != -1)
		{
			// & neighbors <
			if (I_loc - 1 == B_loc || I_loc + 1 == B_loc)
			{
				printf("Invalid background processing format\n");
				return 1;
			}
		}
		if (O_loc != -1)
		{
			// & neighbors >
			if (O_loc - 1 == B_loc || O_loc + 1 == B_loc)
			{
				printf("Invalid background processing format\n");
				return 1;
			}
		}
		if (P_loc != -1)
		{
			// & neighbors |
			if (P_loc - 1 == B_loc || P_loc + 1 == B_loc)
			{
				printf("Invalid background processing format\n");
				return 1;
			}
		}
	}
	return 0;
}

void PrintPrompt()
{
	const char* directory = "PWD";
	const char* machine = "MACHINE";
	const char* user = "USER";    
	char *machine_out;
	char *user_out;
	char *directory_out;
				  
	machine_out = getenv(machine);
	user_out = getenv(user);
	directory_out = getenv(directory);			 
	printf("%s@%s: %s => ", user_out, machine_out, directory_out);
}

void Limits(char** argv)
{
	int childID;
	int status;

	int line_count = 0;

	char file[256] = "/proc/";
	char PID[256];
	char location[256] = "/limits";
	char limit_string[256];

	pid_t childPID = fork();

	if (childPID == 0)
	{
		execv(argv[0], argv);
		printf("Trouble executing: \n");
		PrintArgVector(argv);
		exit(1);
	}
	else if (childPID > 0)
	{
		waitpid(childPID, &status, 0);
		childID = getpid();
		sprintf(PID, "%i", childID);
		strcat(file, PID);
		strcat(file, location);
		FILE* limit_file;
		limit_file = fopen(file, "r");

		while(fgets(limit_string, sizeof(limit_string), limit_file))
		{
			if ((line_count == 3) || (line_count == 7) ||
			    (line_count == 8) || (line_count == 12))
			    {
				printf("%s", limit_string);
			    }
			line_count++;
		}
		fclose(limit_file);
	}
	else
	{
		printf("Fork failed in Limits()\n");
		exit(1);
	}
}

void ETime(char** argv)
{
	int status;
	struct timeval timeofday;
	gettimeofday(&timeofday, NULL);
	double beginning_time=timeofday.tv_sec+(timeofday.tv_usec/1000000.0);

	pid_t childPID = fork();

	if (childPID == 0)
	{
		execv(argv[0], argv);
		printf("Trouble executing: \n");
		PrintArgVector(argv);
		exit(1);
	}
	else if (childPID > 0)
	{
		waitpid(childPID, &status, 0);
	}
	else
	{
		printf("Fork failed in ETime\n");
	}
	gettimeofday(&timeofday, NULL);
	double finishing_time=timeofday.tv_sec+(timeofday.tv_usec/1000000.0);
	printf("Elapsed Time: %f\n", finishing_time-beginning_time);
}
