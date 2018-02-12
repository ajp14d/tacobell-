#include "MyFunctions.h"
#include<stdio.h>
#include<string.h>
#include<unistd.h>

void MemFunc(char** arr)
{
	size_t obj = 0; // store the maximum size of a theoretically possible object of any type (including array)
	
	while (arr[obj] != NULL)
	{
			free(arr[obj]); // free( ) deallocates the memory previously allocated by a call to calloc
			++obj;
	}


	free(arr);
}

void DisplayArgs(char** args)
{
	size_t obj = 0;
	
	while (args[obj] != NULL)
	{
			printf(args[obj]);
			printf("\n"); // to print a new line each time
			++obj;
	}
}

char* DelFunc(char* line, size_t begin, size_t end)
{
	// CHANGE MADE HERE : (start - end) TO (end - start)
	// Need to add 1 to number of characters deleted due to the start
	// and end iterators being inclusive

	size_t charAmount = end - begin + 1;
	int length = strlen(line)+1;
	int lengthUpdate = length - (int)charAmount;
	char* lineUpdate = (char*)calloc(lengthUpdate, sizeof(char)); // calloc allocates the requested memory and returns a pointer to it
	// must put '\0' at the end
	lineUpdate[lengthUpdate - 1] = '\0';
	size_t obj1 = 0;
	size_t obj2 = 0;
	char theChar = line[obj1];
	while (theChar != '\0')
	{
		if (!((obj1 >= begin) && (obj1 <= end)))
		{
			lineUpdate[obj2++] = theChar;
		}
		theChar = line[++obj1];
	}
	free(line);
	line = NULL;
	return lineUpdate;
}

int ExistCheck(const char* file)
{
	if (access(file, F_OK) != -1) // F_OK is the existence check
		return 1;
	else
		return 0;
}

int ExecCheck(const char* file)
{
	struct stat s = {0}; 
	stat(file, &s); // information about the named file and write it to the area pointed to by the 2nd arg
	//printf("%i\n", S_ISREG(s.st_mode));
	int ans = S_ISREG(s.st_mode); // S_ISREG returns non-zero if the file is a regular file, st_mode is mode of file
	return ans;
}

char* LinkString(char* x, const char* y)
{
	size_t lengthTot = strlen(x) + strlen(y) + 1;
	char* stringUpdate = (char*)calloc(lengthTot, sizeof(char));
	int obj1 = strlen(x);
	int obj2 = 0;
	char c = x[obj2];

	while(c != '\0')
	{
		stringUpdate[obj2++] = c;
		c = x[obj2];
	}

	// clear obj2
	obj2 = 0;
	c = y[obj2];
	while (c != '\0')
	{
		stringUpdate[obj1++] = c;
		c = y[++obj2];
	}

	stringUpdate[obj1] = '\0';
	free(x);
	return stringUpdate;
}

char* BPushString(char* x, char y)
{
	size_t lengthTot = strlen(x) + 2;
	char* stringUpdate = (char*)calloc(lengthTot, sizeof(char));
	strcpy(stringUpdate, x);
	stringUpdate[lengthTot-2] = y;
	stringUpdate[lengthTot-1] = '\0';
	free(x);
	return stringUpdate;
}

char* FPushString(char* x, char y)
{
	size_t lengthTot = strlen(x) + 2;
	char* stringUpdate = (char*)calloc(lengthTot, sizeof(char));
	strcpy(stringUpdate, x);
	stringUpdate[0] = y;
	size_t obj = 0;
	while(x[obj] != '\0')
	{
		stringUpdate[obj+1] = x[obj];
		++obj;
	}
	stringUpdate[obj+1] = '\0';
	free(x);
	return stringUpdate;
}

char* RmDir(char* p)
{
	int toRem = strlen(p) - 1;
	int endRem = toRem;
	char c = p[toRem];

	// root directory, do nothing

	if (c == '/' && toRem == 0)
		return p;
	
	// '/' at end of path
	if (c == '/')
	{
		c = p[--toRem];
	}

	
	// search for next '/'
	while (c != '/')
	{
		c = p[--toRem];
	}

	return DelFunc(p, toRem, endRem);
}

int CmdCheck(char** args, int a)
{
	static char* cmdsInclude[6] = {"exit", "echo", "cd", "etime", "limits", NULL}; // static sets scope

	if (strcmp(args[a], "cd") != 0)
	{
		if (CmdVal(args[a]))
			return 1;
	}
	
	if (strcmp(args[a], "cd") == 0)
		return 2;

	size_t obj = 0;
	char* temp = cmdsInclude[obj];

	while (temp != NULL)
	{
		if (strcmp(args[obj], temp) == 0)
		{
			return 3;
		}
		temp = cmdsInclude[++obj];
	}

	return 0;

}	

char* PathMaker(char* s)
{
	char* stringAns = s;

	if ((strlen(stringAns) == 1) && (strcmp(stringAns, "/") == 0))
		return stringAns;

	// check for ~
	if (stringAns[0] == '~')
	{
		stringAns = CharRep(stringAns, 0, 0, getenv("HOME")); // get env var of arg
	}

	// check for .
	if (stringAns[0] == '.' && stringAns[1] != '.')
	{
		// if in root, need to delete both . and /
		if (strcmp(getenv("PWD"), "/") == 0)
			stringAns = CharRep(stringAns, 0, 1, getenv("PWD"));
		// else, only need to delete .
		else
			stringAns = CharRep(stringAns, 0, 0, getenv("PWD"));
	}

	// check for ..
	if (stringAns[0] == '.' && stringAns[1] == '.')
	{
		char* pwd = getenv("PWD");
		char* pwdNew = (char*)calloc(strlen(pwd)+1,sizeof(char));
		strcpy(pwdNew, pwd);
		pwdNew = RmDir(pwdNew);
		// attempt to hardcode fix changing to / from /home
		if (strcmp(pwdNew,"")==0)
		{
			stringAns = CharRep(stringAns, 0, strlen(stringAns)-1, "/");
			free(pwdNew);
			return stringAns;
		}
		stringAns = CharRep(stringAns, 0, 1, pwdNew);
		free(pwdNew);
	}

	// check for . and .. anywhere in string
	size_t obj = 1;
	size_t slash1 = 0;
	size_t slash2 = 0;
	while (stringAns[obj] != '\0')
	{
		if (stringAns[obj] == '.' && stringAns[obj-1] == '.')
		{
			// check for root
			if (obj > 2)
			{
				stringAns = DelFunc(stringAns, slash2, obj); 
				// must start back at 0 to capture '/' locations
				obj = 0;
				slash1 = 0;
				slash2 = 0;
				// empty string, down to root
				if (strcmp(stringAns,"") == 0)
				{
					stringAns = BPushString(stringAns, '/');
					return stringAns;
				}
			}
			else if (strlen(stringAns) > 3)
			{
				stringAns = DelFunc(stringAns, obj-2, obj);
				obj = obj - 3;	// will be -1, ++it at end of loop will make 0
			}
			else
			{
				stringAns = DelFunc(stringAns, obj-1, obj);
				obj = obj - 2;
			}
		}
		// check strlen to make sure no invalid memory read
		else if ((strlen(stringAns) >= 2) && stringAns[obj-1] == '.' && stringAns[obj] == '/')
		{
			stringAns = DelFunc(stringAns, obj-1, obj);
			obj = obj - 2;
		}
		else if (stringAns[obj] == '/')
		{
			slash2 = slash1;
			slash1 = obj;
		}
		++obj;
	}

	return stringAns;
}

char* PathFromEVar(char* s)
{
	// add a '/' to the argument string
	char* sNew = (char*)calloc(strlen(s)+2, sizeof(char));
	strcpy(sNew, "/");
	sNew = LinkString(sNew, s);

	char* p = getenv("PATH");	
	// must create temp path array, because strtok changes its input
	char* pNew = (char*)calloc(strlen(p)+1,sizeof(char));
	strcpy(pNew, p);
	char* temp = strtok(pNew, ":");

	while (temp != NULL)
	{	
		char* ThePath = (char*)calloc(strlen(temp)+1, sizeof(char));
		strcpy(ThePath, temp);

		ThePath = PathClear(ThePath);

		ThePath = LinkString(ThePath, sNew);
		ThePath = PathMaker(ThePath);
		
		if (ExecCheck(ThePath))
		{
			free(s);
			free(sNew);
			free(pNew);
			return ThePath;
		}
		free(ThePath);
		temp = strtok(NULL, ":");
	}

	free(pNew);
	free(sNew);
	return s;

}

int CmdVal(const char* s)
{
	// add a '/' to the argument string
	char* sNew = (char*)calloc(strlen(s)+2, sizeof(char));
	strcpy(sNew, "/");
	sNew = LinkString(sNew, s);

	// check current working directory
	/*char* pwd = getenv("PWD");
	char* pwdNew = (char*)calloc(strlen(pwd)+2, sizeof(char));
	strcpy(pwdNew, pwd);
	pwdNew = PathClear(pwdNew);
	pwdNew = LinkString(pwdNew, sNew);
	if (ExistCheck(pwdNew))
	{
		free(sNew);
		free(pwdNew);
		return 1;
	}
	*/
	
	char* p = getenv("PATH");	
	char* pNew = (char*)calloc(strlen(p)+1,sizeof(char));
	strcpy(pNew, p);

	char* temp = strtok(pNew, ":");

	while (temp != NULL)
	{	
		char* ThePath = (char*)calloc(strlen(temp)+1, sizeof(char));
		strcpy(ThePath, temp);

		ThePath = PathClear(ThePath);

		ThePath = LinkString(ThePath, sNew);
		ThePath = PathMaker(ThePath);

		//printf("%s	%i\n", cur_path, IsExecutable(cur_path));
		if (ExecCheck(ThePath))
		{
			free(ThePath);
			free(sNew);
			free(pNew);
			return 1;
		}
		free(ThePath);
		temp = strtok(NULL, ":");
	}
	free(pNew);
	free(sNew);
	return 0;
}

char* PathClear(char* s)
{
	size_t objLast = strlen(s) - 1;
	if (s[objLast] == '/')
	{
		s = DelFunc(s, objLast, objLast);
	}
	return s;
}

char* CharRep(char* x, size_t begin, size_t end, const char* y)
{
	size_t lengthDel = end - begin + 1;
	size_t xLength = strlen(x);
	size_t yLength = strlen(y);
	size_t lengthNew = xLength - lengthDel + yLength;

	char* stringAns = (char*)calloc(lengthNew + 1, sizeof(char));
	stringAns[lengthNew - 1] = '\0'; 

	int stringAnsObj = 0;
	int xObj = 0;
	int yObj = 0;
	int doneObj = 0;

	if (begin == 0)
		doneObj = end + 1;
	else
		doneObj = end + 1;

	for (stringAnsObj = 0; stringAnsObj < lengthNew; stringAnsObj++)
	{
		if (stringAnsObj < begin)
			stringAns[stringAnsObj] = x[xObj++];
		else if ((stringAnsObj >= begin) && (stringAnsObj <= begin+ yLength -1))
			stringAns[stringAnsObj] = y[yObj++];
		else
			stringAns[stringAnsObj] = x[doneObj++];
	}
	free(x);
	return stringAns;
}

int CharCheck(const char* s, char c)
{
	size_t obj = 0;
	while (s[obj] != '\0')
	{
		if (s[obj] == c)
		{
			return 1;
		}
		++obj;
	}
	return 0;
}

int StringCheck(char** argv, const char* s)
{
	int obj = 0;
	while (argv[obj] != NULL)
	{
		if (strcmp(argv[obj], s) == 0)
		{
			return obj;
		}
		++obj;
	}
	return -1;
}

int GetSize(char** argv)
{
	if (argv == NULL)
		return 0;
	int obj = 0;
	while(argv[obj] != NULL)
	{
		++obj;
	}
	return obj;
}

char** PBackArr(char** argv, const char* s)
{
	char** vecNew = (char**)calloc(GetSize(argv)+2,sizeof(char*));
	int obj = 0;
	while (argv[obj] != NULL)
	{
		vecNew[obj] = (char*)calloc(strlen(argv[obj])+1, sizeof(char));
		strcpy(vecNew[obj], argv[obj]);
		++obj;
	}
	vecNew[obj] = (char*)calloc(strlen(s)+1, sizeof(char));
	strcpy(vecNew[obj], s);
	vecNew[obj+1] = NULL;
	MemFunc(argv);
	return vecNew;
}

char** RemoveArr(char** argv, int a)
{
	if (a > GetSize(argv))
	{
		return argv;
	}
	int obj1 = 0;
	int obj2 = 0;

	char** vecNew = (char**)calloc(GetSize(argv),sizeof(char*));
	while (argv[obj1] != NULL)
	{
		if (obj1 != a)
		{
			vecNew[obj2] = (char*)calloc(strlen(argv[obj1])+1, sizeof(char));
			strcpy(vecNew[obj2], argv[obj1]);
			++obj2;
		}
		++obj1;
	}
	vecNew[obj2] = NULL;
	MemFunc(argv);
	return vecNew;
}

char* Convert(char** argv)
{
	int obj = 0;
	// empty argv
	if (argv[obj] == NULL)
		return NULL;
	char* ans = (char*)calloc(1, sizeof(char));
	ans[0] = '\0';
	while (argv[obj] != NULL)
	{
		ans = LinkString(ans, argv[obj]);
		++obj;
		if (argv[obj] != NULL)	
			ans = BPushString(ans, ' ');
	}
	return ans;
}

int CountStr(char** argv, const char* s)
{
	int obj = 0;
	int track = 0;
	while (argv[obj] != NULL)
	{
		if (strcmp(argv[obj], s) == 0)
		{
			++track;
		}
		++obj;
	}
	return track;
}

void ChangeDirectory(const char* dir)
{
	int ret = chdir(dir);
	switch(ret){
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



void ExecuteExternal(char** argv, int background, char* cmd)
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
		switch(background){
		default:
		{
			waitpid(pid, &status, WNOHANG);
			handleQueue(newPro(pid, -1, cmd));
			//kill(pid, SIGSTOP);
			//Need to implement a pause here for things like text editors
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

char** ExecuteExternalWithInput(char** argv, int I_loc, int background)
{
	char* filename = (char*)calloc(strlen(argv[I_loc+1])+1, sizeof(char));
	strcpy(filename, argv[I_loc+1]);

	argv = RemoveArr(argv, I_loc);
	argv = RemoveArr(argv, I_loc);
				
	// update background iterator
	background = StringCheck(argv, "&");
	char* cmd = Convert(argv);
	if (background != -1)
	{
		argv = RemoveArr(argv, background);
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

	argv = RemoveArr(argv, O_loc);
	argv = RemoveArr(argv, O_loc);
			
	// update background iterator
	background = StringCheck(argv, "&");
	char* cmd = Convert(argv);
	if (background != -1)
	{
		argv = RemoveArr(argv, background);
	}
	IORedirect(argv, 0, filename, background, cmd);
	free(filename);
	free(cmd);
	
	return argv;
}

char** ExecuteExternalWithPipe(char** argv, int pipe_count, int background)
{
	char* cmd = Convert(argv);
	if (background != -1)
	{
		argv = RemoveArr(argv, background);
	}

	if (pipe_count == 1 || pipe_count == 2 || pipe_count == 3)
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
		switch(pipe_count){
			
			case 1:
        {
            while(argv[it] != NULL)
            {
                argv2 = PBackArr(argv2, argv[it]);
                ++it;
            }
            OnePipe(argv1, argv2, background, cmd);
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
            TwoPipe(argv1, argv2, argv3, background, cmd);
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
            ThreePipe(argv1, argv2, argv3, argv4, background, cmd);
            MemFunc(argv1);
            MemFunc(argv2);
            MemFunc(argv3);
            MemFunc(argv4);
		break;
        }
}
	free(cmd);
	return argv;
}

void IORedirect(char** argv, int dir, char* filename, int background, char* cmd)
{
	// output redirection
	switch(dir){
	case 0:
	{
		int status;
		int fd = open(filename, O_CREAT|O_WRONLY|O_TRUNC, 0777);
		if (fd == -1)
		{
			printf("Error. Cannot open filename: %s\n", filename);
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
			switch(background){
			default:
			{
				waitpid(pid, &status, WNOHANG);
				handleQueue(newPro(pid, -1, cmd));
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
		int fd = open(filename, O_RDONLY);
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
		
			switch(background){
			default:
			{
				waitpid(pid, &status, WNOHANG);
				handleQueue(newPro(pid, -1, cmd));
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

int CheckForIOandPipeErrors(char** argv)
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

int CheckForBackgroundErrors(char** argv)
{
	int I_loc = StringCheck(argv, "<");
	int O_loc = StringCheck(argv, ">");
	int P_loc = StringCheck(argv, "|");
	int B_loc = StringCheck(argv, "&");
	
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
	char location[256] = "/io";
	char limit_string[256];

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

