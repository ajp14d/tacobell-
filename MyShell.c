#include "MyShell.h"

int main()
{
	startMyShell();
	return 0;
}

void startMyShell()
{
	int start = 1;
	char* str;
	char** argv;
	Init();
	while (start)
	{
		KillZombies();
		checkQueue();
		displayPrompt();
		str = GetInput();
		argv = ParseI(str);
		
		if ((argv[0] == NULL) || errorsPipeIO(argv) || errorsBackground(argv))
		{
			
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
			
			if (GetSize(argv) == 2)
					dirChange(argv[1]);
			else if (GetSize(argv) < 2)
					dirChange(getenv("HOME"));
			else	
				printf("There are too many arguments after cd\n");
			
			
			
		}
		else if (strcmp(argv[0], "io") == 0)
		{
			if (GetSize(argv) > 1)
			{
				argv = RemoveArr(argv, 0);
				ioCmd(argv);
			}
		}
		else if (strcmp(argv[0], "etime") == 0)
		{
			if (GetSize(argv) > 1)
			{
				
				argv = RemoveArr(argv, 0);
				etimeCmd(argv);
			}
		}
		else if (ExecCheck(argv[0]))
		{
			int back = StringCheck(argv, "&");
			int in = StringCheck(argv, "<");
			int out = StringCheck(argv, ">");
			int numpipe = CountStr(argv, "|");
			if (in != -1)
			{
				argv = externIn(argv, in, back);
			}
			else if (out != -1)
			{
				argv = externOut(argv, out, back);
			}
			else if (numpipe > 0)
			{
				argv = externPipe(argv, numpipe, back);
			}
			else
			{
				char* c = Convert(argv);
				if (back != -1)
				{
					argv = RemoveArr(argv, back);
				}
				theExtern(argv, back, c);
				free(c);
			}
		}


		MemFunc(argv);
	}
}

void dirChange(const char* dir)
{
	int ans = chdir(dir);
	switch(ans){
	case 0:
	{
		setenv("PWD", dir, 1);
		break;
	}
	default:
	{
		printf("No such directory\n");
		break;
	}
	}
}



void theExtern(char** argv, int back, char* c)
{
	int status;
	pid_t pid = fork();
	switch(pid){
	case -1:
	{
		printf("Error on fork\n");
		exit(1);
		break;
	}
	case 0:
	{
		execv(argv[0], argv);
		printf("Trouble executing: \n");
		DisplayArgs(argv);
		exit(1);
		break;
	}
	default:
	{
		switch(back){
		default:
		{
			waitpid(pid, &status, WNOHANG);
			handleQueue(newPro(pid, -1, c));
			break;
		}
		case -1:
		{
			waitpid(pid, &status, 0);
			break;
		}
		}
		break;
	}
	}
}

char** externIn(char** argv, int in, int back)
{
	char* file = (char*)calloc(strlen(argv[in+1])+1, sizeof(char));
	strcpy(file, argv[in+1]);

	argv = RemoveArr(argv, in);
	argv = RemoveArr(argv, in);
				
	// update background iterator
	back = StringCheck(argv, "&");
	char* c = Convert(argv);
	if (back != -1)
	{
		argv = RemoveArr(argv, back);
	}
	handleIO(argv, 1, file, back, c);
	free(file);
	free(c);
	
	return argv;
}

char** externOut(char** argv, int out, int back)
{
	char* file = (char*)calloc(strlen(argv[out+1])+1, sizeof(char));
	strcpy(file, argv[out+1]);

	argv = RemoveArr(argv, out);
	argv = RemoveArr(argv, out);
			
	// update background iterator
	back = StringCheck(argv, "&");
	char* c = Convert(argv);
	if (back != -1)
	{
		argv = RemoveArr(argv, back);
	}
	handleIO(argv, 0, file, back, c);
	free(file);
	free(c);
	
	return argv;
}

char** externPipe(char** argv, int numpipe, int back)
{
	char* c = Convert(argv);
	if (back != -1)
	{
		argv = RemoveArr(argv, back);
	}
	

	if (numpipe == 1 || numpipe == 2 || numpipe == 3)
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
        argv1 = PBackArr(argv1, argv[it]);
        ++it;
    }
    ++it;
		switch(numpipe){
			
			case 1:
        {
            while(argv[it] != NULL)
            {
                argv2 = PBackArr(argv2, argv[it]);
                ++it;
            }
            //OnePipe(argv1, argv2, background, cmd);
		
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
			if (back != -1)
			{
				waitpid(c2PID, &status, WNOHANG);
				waitpid(c1PID, &status, WNOHANG);
				handleQueue(newPro(c1PID, c2PID, c));
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
		
		
            MemFunc(argv1);
            MemFunc(argv2);
		break;
        }
			case 2:
        {
            while(strcmp(argv[it], "|") != 0)
            {
                argv2 = PBackArr(argv2, argv[it]);
                ++it;
            }
            ++it;
            while(argv[it] != NULL)
            {
                argv3 = PBackArr(argv3, argv[it]);
                ++it;
            }
            //TwoPipe(argv1, argv2, argv3, background, cmd);
		
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
		
		
            MemFunc(argv1);
            MemFunc(argv2);
            MemFunc(argv3);
		break;
        }
			case 3:
        {
            while(strcmp(argv[it], "|") != 0)
            {
                argv2 = PBackArr(argv2, argv[it]);
                ++it;
            }
            ++it;
            while(strcmp(argv[it], "|") != 0)
            {
                argv3 = PBackArr(argv3, argv[it]);
                ++it;
            }
            ++it;
            while(argv[it] != NULL)
            {
                argv4 = PBackArr(argv4, argv[it]);
                ++it;
            }
            //ThreePipe(argv1, argv2, argv3, argv4, background, cmd);
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
		
		
		
		
            MemFunc(argv1);
            MemFunc(argv2);
            MemFunc(argv3);
            MemFunc(argv4);
		break;
        }

	}
}
	free(c);
	return argv;
	
}

void handleIO(char** argv, int dir, char* file, int back, char* c)
{
	// output redirection
	switch(dir){
	case 0:
	{
		int status;
		int fd = open(file, O_CREAT|O_WRONLY|O_TRUNC, 0777);
		if (fd == -1)
		{
			printf("Error. Cannot open filename: %s\n", file);
			exit(1);
		}
		pid_t pid = fork();

		switch(checkZero(pid)){
		case 2:
		{
			close(1);
			dup(fd);
			close(fd);
			execv(argv[0], argv);
			printf("Unknown command in IORedirect\n");
			exit(1);
			break;
		}
		case 1:
		{
			switch(back){
			default:
			{
				waitpid(pid, &status, WNOHANG);
				handleQueue(newPro(pid, -1, c));
				break;
			}
			case -1:
			{
				waitpid(pid, &status, 0);
				break;
			}
			}
			close(fd);
			break;
		}
		case 0:
		{
			printf("Fork failed in IORedirect\n");
			exit(1);
			break;
		}
		}
		break;
	}
	case 1:
	{
		int status;
		int fd = open(file, O_RDONLY);
		switch(fd){
              	  case -1:
		  {
			printf("Error, cannot access file in IORedirect!\n");
			exit(1);
			break;
		  }
		}
		pid_t pid = fork();
		
		switch(checkZero(pid)){
		case 2:
		{
			close(0);
			dup(fd);
			close(fd);
			execv(argv[0], argv);

			printf("Unknown command in IORedirect\n");
			exit(1);
			break;
		}
		case 1:
		{
		
			switch(back){
			default:
			{
				waitpid(pid, &status, WNOHANG);
				handleQueue(newPro(pid, -1, c));
				break;
			}
			case -1:
			{
				waitpid(pid, &status, 0);
				break;
			}
			}
			close(fd);
			break;
		}
		case 0:
		{
			printf("Fork() failed in IORedirect\n");
			exit(1);
			break;
		}
		}
		break;
	}
	}
}

int errorsPipeIO(char** argv)
{
	
	switch(GetSize(argv)){
	case 1:
	{
		if ((StringCheck(argv, "<") != -1) ||
			(StringCheck(argv, ">") != -1) ||
			(StringCheck(argv, "|") != -1))
		{
			printf("Requires more than 1 argument\n");
			return 1;
		}
		break;
	}
	default:
	{
		if ((StringCheck(argv, "<") == 0) ||
			(StringCheck(argv, ">") == 0) ||
			(StringCheck(argv, "|") == 0))
		{
			printf("Error, IO redirection or pipe cannot appear at beginning of statement\n");
			return 1;
		}
		else if ((StringCheck(argv, "<") == GetSize(argv)-1) ||
				 (StringCheck(argv, ">") == GetSize(argv)-1) ||
				 (StringCheck(argv, "|") == GetSize(argv)-1))
		{
			printf("Error, IO redirection or pipe cannot appear at end of statement\n");
			return 1;
		}
		break;
	}
	}
	return 0;
}

int errorsBackground(char** argv)
{
	int in = StringCheck(argv, "<");
	int out = StringCheck(argv, ">");
	int pipe = StringCheck(argv, "|");
	int back = StringCheck(argv, "&");
	
	if (back == -1)
	{
		return 0;
	}
	
	if ((in == -1) &&
		(out == -1) &&
		(pipe == -1))
	{
		return 0;
	}
	else
	{
		if (in != -1)
		{
			// & neighbors <
			if (in - 1 == back || in + 1 == back)
			{
				printf("Invalid background processing format\n");
				return 1;
			}
		}
		if (out != -1)
		{
			// & neighbors >
			if (out - 1 == back || out + 1 == back)
			{
				printf("Invalid background processing format\n");
				return 1;
			}
		}
		if (pipe != -1)
		{
			// & neighbors |
			if (pipe - 1 == back || pipe + 1 == back)
			{
				printf("Invalid background processing format\n");
				return 1;
			}
		}
	}
	return 0;
}

void displayPrompt()
{
	const char* dir = "PWD";
	const char* mach = "MACHINE";
	const char* user = "USER";    
	char *mprint;
	char *uprint;
	char *dprint;
				  
	mprint = getenv(mach);
	uprint = getenv(user);
	dprint = getenv(dir);			 
	printf("%s@%s: %s => ", uprint, mprint, dprint);
}

void ioCmd(char** argv)
{
	int childID;
	int status;

	int numline = 0;

	char file[256] = "/proc/";
	char PID[256];
	char location[256] = "/io";
	char iostr[256];

	pid_t childPID; 
	childPID = fork();

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
	{
		waitpid(childPID, &status, 0);
		childID = getpid();
		sprintf(PID, "%i", childID);
		strcat(file, PID);
		strcat(file, location);
		FILE* iofile;
		iofile = fopen(file, "r");

		while(fgets(iostr, sizeof(iostr), iofile))
		{
			if ((numline == 3) || (numline == 7) ||
			    (numline == 8) || (numline == 12))
			    {
				printf("%s", iostr);
			    }
			numline++;
		}
		fclose(iofile);
		break;
	}
	case 0:
	{
		printf("Fork failed in Limits()\n");
		exit(1);
		break;
	}
	}
}

void etimeCmd(char** argv)
{
	int now;
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
		waitpid(childPID, &now, 0);
		break;
	  case 0:
		printf("Fork failed in ETime\n");
		break;
	}


	gettimeofday(&timeofday, NULL);
	double finishing_time=timeofday.tv_sec+(timeofday.tv_usec/1000000.0);
	printf("Elapsed Time: %f\n", finishing_time-beginning_time);
}

int checkZero(int tocheck)
{
	if (tocheck == 0)
		return 2;
	else if (tocheck > 0)
		return 1;
	else
		return 0;
}
	

void KillZombies() {

    pid_t pid;

    // Kill processes as long as we keep finding them
    while ( ( pid = waitpid( -1, 0, WNOHANG ) )  )
        
        // No zombie processes are found
        if ( pid == -1 || pid == 0 )
            break;

}


