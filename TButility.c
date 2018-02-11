#include "utility.h"
#include<stdio.h>
#include<string.h>
#include<unistd.h>

void MemFunc(char** arr)   //BigFree(char** array)
{
	size_t it = 0;
	while (array[it] != NULL)
	{
		free(array[it]);
		++it;
	}
	free(array);
}

void PrintArgVector(char** args)
{
	size_t it = 0;
	
	while (args[it] != NULL)
	{
			printf(args[it]);
			printf("\n");
			++it;
	}
}

char* DeleteCharacter(char* line, size_t start, size_t end)
{
	// CHANGE MADE HERE : (start - end) TO (end - start)
	// Need to add 1 to number of characters deleted due to the start
	// and end iterators being inclusive
	size_t number_characters = end - start + 1;
	int current_length = strlen(line)+1;
	int new_length = current_length - (int)number_characters;
	char* new_line = (char*)calloc(new_length, 
		sizeof(char));
	// must put '\0' at the end
	new_line[new_length - 1] = '\0';
	size_t it1 = 0;
	size_t it2 = 0;
	char cur_char = line[it1];
	while (cur_char != '\0')
	{
		if (!((it1 >= start) && (it1 <= end)))
		{
			new_line[it2++] = cur_char;
		}
		cur_char = line[++it1];
	}
	free(line);
	line = NULL;
	return new_line;
}

int FileExist(const char* filename)
{
	if (access(filename, F_OK) != -1)
		return 1;
	else
		return 0;
}

int IsExecutable(const char* filename)
{
	struct stat s = {0};
	stat(filename, &s);
	//printf("%i\n", S_ISREG(s.st_mode));
	int ret = S_ISREG(s.st_mode);
	return ret;
}

char* DynStrCat(char* dest, const char* source)
{
	size_t total_len = strlen(dest) + strlen(source) + 1;
	char* new_str = (char*)calloc(total_len, sizeof(char));
	int it1 = strlen(dest);
	int it2 = 0;
	char c = dest[it2];

	while(c != '\0')
	{
		new_str[it2++] = c;
		c = dest[it2];
	}

	// clear it2
	it2 = 0;
	c = source[it2];
	while (c != '\0')
	{
		new_str[it1++] = c;
		c = source[++it2];
	}

	new_str[it1] = '\0';
	free(dest);
	return new_str;
}

char* DynStrPushBack(char* dest, char c)
{
	size_t total_len = strlen(dest) + 2;
	char* new_str = (char*)calloc(total_len, sizeof(char));
	strcpy(new_str, dest);
	new_str[total_len-2] = c;
	new_str[total_len-1] = '\0';
	free(dest);
	return new_str;
}

char* DynStrPushFront(char* dest, char c)
{
	size_t total_len = strlen(dest) + 2;
	char* new_str = (char*)calloc(total_len, sizeof(char));
	strcpy(new_str, dest);
	new_str[0] = c;
	size_t it = 0;
	while(dest[it] != '\0')
	{
		new_str[it+1] = dest[it];
		++it;
	}
	new_str[it+1] = '\0';
	free(dest);
	return new_str;
}

char* RemoveLastDir(char* path)
{
	int rIt = strlen(path) - 1;
	int endIt = rIt;
	char c = path[rIt];

	// root directory, do nothing
	if (c == '/' && rIt == 0)
		return path;
	
	// '/' at end of path
	if (c == '/')
	{
		c = path[--rIt];
	}
	
	// search for next '/'
	while (c != '/')
	{
		c = path[--rIt];
	}

	return DeleteCharacter(path, rIt, endIt);
}

int IsCommand(char** args, int i)
{
	static char* built_ins[5] = {"exit", "echo", "etime", "limits", NULL};

	if (ValidExternalCommand(args[i]))
		return 1;

	if (strcmp(args[i], "cd") == 0)
		return 2;

	size_t it = 0;
	char* tmp = built_ins[it];

	while (tmp != NULL)
	{
		if (strcmp(args[i], tmp) == 0)
		{
			return 3;
		}
		tmp = built_ins[++it];
	}

	return 0;

}	

char* BuildPath(char* str)
{
	char* ret_str = str;

	if ((strlen(ret_str) == 1) && (strcmp(ret_str, "/") == 0))
		return ret_str;

	// check for ~
	if (ret_str[0] == '~')
	{
		ret_str = ReplaceSubStr(ret_str, 0, 0, getenv("HOME"));
	}

	// check for .
	if (ret_str[0] == '.' && ret_str[1] != '.')
	{
		// if in root, need to delete both . and /
		if (strcmp(getenv("PWD"), "/") == 0)
			ret_str = ReplaceSubStr(ret_str, 0, 1, getenv("PWD"));
		// else, only need to delete .
		else
			ret_str = ReplaceSubStr(ret_str, 0, 0, getenv("PWD"));
	}

	// check for ..
	if (ret_str[0] == '.' && ret_str[1] == '.')
	{
		char* pwd = getenv("PWD");
		char* dyn_pwd = (char*)calloc(strlen(pwd)+1,sizeof(char));
		strcpy(dyn_pwd, pwd);
		dyn_pwd = RemoveLastDir(dyn_pwd);
		// attempt to hardcode fix changing to / from /home
		if (strcmp(dyn_pwd,"")==0)
		{
			ret_str = ReplaceSubStr(ret_str, 0, strlen(ret_str)-1, "/");
			free(dyn_pwd);
			return ret_str;
		}
		ret_str = ReplaceSubStr(ret_str, 0, 1, dyn_pwd);
		free(dyn_pwd);
	}

	// check for . and .. anywhere in string
	size_t it = 1;
	size_t last_slash = 0;
	size_t last_last_slash = 0;
	while (ret_str[it] != '\0')
	{
		if (ret_str[it] == '.' && ret_str[it-1] == '.')
		{
			// check for root
			if (it > 2)
			{
				ret_str = DeleteCharacter(ret_str, last_last_slash, it); 
				// must start back at 0 to capture '/' locations
				it = 0;
				last_slash = 0;
				last_last_slash = 0;
				// empty string, down to root
				if (strcmp(ret_str,"") == 0)
				{
					ret_str = DynStrPushBack(ret_str, '/');
					return ret_str;
				}
			}
			else if (strlen(ret_str) > 3)
			{
				ret_str = DeleteCharacter(ret_str, it-2, it);
				it = it - 3;	// will be -1, ++it at end of loop will make 0
			}
			else
			{
				ret_str = DeleteCharacter(ret_str, it-1, it);
				it = it - 2;
			}
		}
		// check strlen to make sure no invalid memory read
		else if ((strlen(ret_str) >= 2) && ret_str[it-1] == '.' && ret_str[it] == '/')
		{
			ret_str = DeleteCharacter(ret_str, it-1, it);
			it = it - 2;
		}
		else if (ret_str[it] == '/')
		{
			last_last_slash = last_slash;
			last_slash = it;
		}
		++it;
	}

	return ret_str;
}

char* PathEnvBuildPath(char* str)
{
	// add a '/' to the argument string
	char* dyn_str = (char*)calloc(strlen(str)+2, sizeof(char));
	strcpy(dyn_str, "/");
	dyn_str = DynStrCat(dyn_str, str);

	char* path = getenv("PATH");	
	// must create temp path array, because strtok changes its input
	char* dyn_path = (char*)calloc(strlen(path)+1,sizeof(char));
	strcpy(dyn_path, path);
	char* tmp = strtok(dyn_path, ":");

	while (tmp != NULL)
	{	
		char* cur_path = (char*)calloc(strlen(tmp)+1, sizeof(char));
		strcpy(cur_path, tmp);

		cur_path = CleanPathDirectory(cur_path);

		cur_path = DynStrCat(cur_path, dyn_str);
		cur_path = BuildPath(cur_path);
		
		if (IsExecutable(cur_path))
		{
			free(str);
			free(dyn_str);
			free(dyn_path);
			return cur_path;
		}
		free(cur_path);
		tmp = strtok(NULL, ":");
	}

	free(dyn_path);
	free(dyn_str);
	return str;

}

int ValidExternalCommand(const char* str)
{
	// add a '/' to the argument string
	char* dyn_str = (char*)calloc(strlen(str)+2, sizeof(char));
	strcpy(dyn_str, "/");
	dyn_str = DynStrCat(dyn_str, str);

	// check current working directory
	/*char* pwd = getenv("PWD");
	char* dyn_pwd = (char*)calloc(strlen(pwd)+2, sizeof(char));
	strcpy(dyn_pwd, pwd);
	dyn_pwd = CleanPathDirectory(dyn_pwd);
	dyn_pwd = DynStrCat(dyn_pwd, dyn_str);
	if (FileExist(dyn_pwd))
	{
		free(dyn_str);
		free(dyn_pwd);
		return 1;
	}
	*/
	
	char* path = getenv("PATH");	
	char* dyn_path = (char*)calloc(strlen(path)+1,sizeof(char));
	strcpy(dyn_path, path);

	char* tmp = strtok(dyn_path, ":");

	while (tmp != NULL)
	{	
		char* cur_path = (char*)calloc(strlen(tmp)+1, sizeof(char));
		strcpy(cur_path, tmp);

		cur_path = CleanPathDirectory(cur_path);

		cur_path = DynStrCat(cur_path, dyn_str);
		cur_path = BuildPath(cur_path);

		//printf("%s	%i\n", cur_path, IsExecutable(cur_path));
		if (IsExecutable(cur_path))
		{
			free(cur_path);
			free(dyn_str);
			free(dyn_path);
			return 1;
		}
		free(cur_path);
		tmp = strtok(NULL, ":");
	}
	free(dyn_path);
	free(dyn_str);
	return 0;
}

char* CleanPathDirectory(char* str)
{
	size_t endIt = strlen(str) - 1;
	if (str[endIt] == '/')
	{
		str = DeleteCharacter(str, endIt, endIt);
	}
	return str;
}

char* ReplaceSubStr(char* dest, size_t start, size_t end, const char* source)
{
	size_t deletion_len = end - start + 1;
	size_t dest_len = strlen(dest);
	size_t source_len = strlen(source);
	size_t new_len = dest_len - deletion_len + source_len;

	char* ret_str = (char*)calloc(new_len + 1, sizeof(char));
	ret_str[new_len - 1] = '\0'; 

	int ret_str_it = 0;
	int dest_it = 0;
	int source_it = 0;
	int final_it = 0;

	if (start == 0)
		final_it = end + 1;
	else
		final_it = end + 1;

	for (ret_str_it = 0; ret_str_it < new_len; ret_str_it++)
	{
		if (ret_str_it < start)
			ret_str[ret_str_it] = dest[dest_it++];
		else if ((ret_str_it >= start) && (ret_str_it <= start+source_len-1))
			ret_str[ret_str_it] = source[source_it++];
		else
			ret_str[ret_str_it] = dest[final_it++];
	}
	free(dest);
	return ret_str;
}

int ContainsChar(const char* str, char c)
{
	size_t it = 0;
	while (str[it] != '\0')
	{
		if (str[it] == c)
		{
			return 1;
		}
		++it;
	}
	return 0;
}

int VecContainsStr(char** argv, const char* str)
{
	int it = 0;
	while (argv[it] != NULL)
	{
		if (strcmp(argv[it], str) == 0)
		{
			return it;
		}
		++it;
	}
	return -1;
}

int ArraySize(char** argv)
{
	if (argv == NULL)
		return 0;
	int it = 0;
	while(argv[it] != NULL)
	{
		++it;
	}
	return it;
}

char** ArrayPushBack(char** argv, const char* str)
{
	char** new_argv = (char**)calloc(ArraySize(argv)+2,sizeof(char*));
	int it = 0;
	while (argv[it] != NULL)
	{
		new_argv[it] = (char*)calloc(strlen(argv[it])+1, sizeof(char));
		strcpy(new_argv[it], argv[it]);
		++it;
	}
	new_argv[it] = (char*)calloc(strlen(str)+1, sizeof(char));
	strcpy(new_argv[it], str);
	new_argv[it+1] = NULL;
	BigFree(argv);
	return new_argv;
}

char** ArrayRemoveElement(char** argv, int index)
{
	if (index > ArraySize(argv))
	{
		return argv;
	}
	int it1 = 0;
	int it2 = 0;

	char** new_argv = (char**)calloc(ArraySize(argv),sizeof(char*));
	while (argv[it1] != NULL)
	{
		if (it1 != index)
		{
			new_argv[it2] = (char*)calloc(strlen(argv[it1])+1, sizeof(char));
			strcpy(new_argv[it2], argv[it1]);
			++it2;
		}
		++it1;
	}
	new_argv[it2] = NULL;
	BigFree(argv);
	return new_argv;
}

char* ArgvToString(char** argv)
{
	int it = 0;
	// empty argv
	if (argv[it] == NULL)
		return NULL;
	char* ret = (char*)calloc(1, sizeof(char));
	ret[0] = '\0';
	while (argv[it] != NULL)
	{
		ret = DynStrCat(ret, argv[it]);
		++it;
		if (argv[it] != NULL)	
			ret = DynStrPushBack(ret, ' ');
	}
	return ret;
}

int ArgvCountSymbol(char** argv, const char* str)
{
	int it = 0;
	int count = 0;
	while (argv[it] != NULL)
	{
		if (strcmp(argv[it], str) == 0)
		{
			++count;
		}
		++it;
	}
	return count;
}
