#include "parse.h"
#include "utility.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char* GetInput()                         //char* ReadInput()
{                                              //gets whatever input the user puts in
	size_t BUFFER_SIZE = 512;
	char* input_s = (char*)calloc(BUFFER_SIZE, sizeof(char));
	if(fgets(input_s, (int)BUFFER_SIZE, stdin))                                  
		return input_s;                       	                                                   
	else                                        
		return NULL;
}

char** ParseI(char* input)                           //ParseInput
{                                               //seperates and stores the values
	input = PWhitespace(input);       //ParseWhitespace
	char** split_arg = ParseArguments(input);         ///////////////////   //split_args
	
	if(split_arg[0] != NULL)          //if '&' is in front remove it         //check for leading '&' and remove if present
	{
		if(strcmp(split_arg[0], "&") == 0)
			split_arg = RemoveArr(split_arg, 0);
	}
	
	split_arg = ExpandVariables(split_arg);           //////////////////////
	split_arg = ResolvePaths(split_arg);             ///////////////
	
	// debug message
	//DisplayArgs(char** args);      //PrintArgVector(split_arg);        //////////////////
  
	free(input);    //free the input 
	return split_arg;      //return the newly parsed argument 
}

char* PWhitespace(char* line)                    //ParseWhitespace
{
  int wspace_count = 0;      //whitespace_count
	size_t that = 0;       //it = 0
	char current_char = line[that];     //cur_char
	
	// delete leading whitespace
	while (cur_char != '\0' && (cur_char == ' ' || cur_char == '\t' || cur_char == '\n'))
	{
		cur_char = line[++it];
		whitespace_count++;
	}
	if (it > 0)
		line = DelFunc(line, it-whitespace_count, it-1);
	
	// check for empty string
	//if (strcmp(line,"")==0)
	//	return line;
	
	// debug message
	/*printf("Made it through leading whitespace and deleted %i characters...\n",
		whitespace_count);
	printf(line);
	printf("\n");*/
	
	it = 0;
	whitespace_count = 0;
	
	// need flag to determine if currently operating on
	// trailing whitespace
	int contains_trailing = 0;
	// delete intermediate extra whitespace
	while (cur_char != '\0')
	{
		cur_char = line[it++];
		while (cur_char == ' ' || cur_char == '\t' || cur_char == '\n')
		{
			whitespace_count++;
			cur_char = line[it++];
			if (cur_char == '\0')
				contains_trailing = 1;
		}
		if (contains_trailing == 1)
		{
			if (whitespace_count > 0)
			{
				line = DelFunc(line, it-whitespace_count-1, it-2);
			}
			break;
		}
		else if (whitespace_count > 1)
		{
			line = DelFunc(line, it-whitespace_count-1, it-3);
			// must update iterator if array is changed through deletion
			it = it - (whitespace_count - 1);
		}
		whitespace_count = 0;
	}
	
	// debug message
	/*printf("Made it through intermediate whitespace...\n",
		whitespace_count);
	printf(line);
	printf("%i",strlen(line));
	printf("\n");*/
	
	// bug on return
	// current guess is somewhere writing over the return address
	// (buffer overflow)
	return line;
}

char** ParseArguments(char* input)
{
	size_t it = 0;
	char c = input[it];
	int token_count = 1;

	while (c != '\0')
	{
		if ((c == ' ') || (c == '\n') || (c == '\t'))
		{
			token_count++;
		}
		c = input[++it];
	}

	int i = 0;

	char** ret = (char**)calloc(token_count + 1, sizeof(char*));

	char* tmp = strtok(input, " \n\t");

	if (tmp != NULL)
	{
		ret[i] = (char*)calloc(strlen(tmp)+1, sizeof(char));
		strcpy(ret[i], tmp);
	}
	for (i = 1; i < token_count; i++)
	{
		tmp = strtok(NULL, " \n\t");
		ret[i] = (char*)calloc(strlen(tmp)+1, sizeof(char));
		strcpy(ret[i], tmp);
	}

	ret[i] = NULL;

	return ret;
}

char** ResolvePaths(char** args)
{
	int arg_it = 0;		// iterator
	int cmd_type = 0;	// parent cmd type
	//int cur_type;		// current type of args[arg_it]				[FIXED COMPILER WARNING UNUSED VAR]
	int cmd_it = 0;		// it to point to current command
	int new_cmd = 1;	// flag for new cmd (after | < or >)
	char* cur_cmd = args[0];


	while(args[arg_it] != NULL)
	{
		//cur_type = IsCommand(args,arg_it);						[FIXED COMPILER WARNING UNUSED VAR]
		//printf("%s	%i\n", args[arg_it], cur_type);
		if (new_cmd == 1)
		{
			cmd_type = CmdCheck(args, arg_it);  
			cur_cmd = args[arg_it];
			cmd_it = arg_it;
			new_cmd = 0;
		}
		else
		{
			if ((strcmp(args[arg_it], "|") == 0) || 
			    (strcmp(args[arg_it], "<") == 0) ||
			    (strcmp(args[arg_it], ">") == 0))
			    {
				new_cmd = 1;
				++arg_it;
				continue;
			    }
		}
		
		// cd
		if (cmd_type == 2)
		{
			if (arg_it == (cmd_it + 1))
			{
				if (!CharCheck(args[arg_it], '/'))
				{
					if (!CharCheck(args[arg_it], '~') && !CharCheck(args[arg_it], '.'))
					{
						args[arg_it] = FPushString(args[arg_it], '/');
						args[arg_it] = FPushString(args[arg_it], '.');
					}
				}
				else
				{
					if (args[arg_it][0] != '/' &&
						args[arg_it][0] != '.' &&
						args[arg_it][0] != '~')
					{
						args[arg_it] = FPushString(args[arg_it], '/');
						args[arg_it] = FPushString(args[arg_it], '.');
					}
				}
				args[arg_it] = PathMaker(args[arg_it]);
			}
		}

		// etime and limits
		else if (cmd_type == 3)
		{
			if ((strcmp(cur_cmd, "etime") == 0) || (strcmp(cur_cmd, "limits") == 0))
			{
				if (arg_it == (cmd_it + 1))
				{
					if (CharCheck(args[arg_it], '/') == 1)
					{
						args[arg_it] = PathMaker(args[arg_it]);
					}
					else
					{
						args[arg_it] = PathFromEVar(args[arg_it]);
					}

					//args[arg_it] = BuildPath(args[arg_it]);
				}
			}
		}

		// external commands
		else if (cmd_type == 1)
		{
			if (arg_it == cmd_it)
			{
				if (CharCheck(args[arg_it], '/') == 1)
				{
					args[arg_it] = PathMaker(args[arg_it]);
				}
				else
				{
					args[arg_it] = PathFromEVar(args[arg_it]);
				}
			}
		}
	
		
		++arg_it;
	}

	return args;
}

char** ExpandVariables(char** args)
{
	size_t arg_it = 0;
	size_t str_it = 0;
	//char* str = args[arg_it];

	while (args[arg_it] != NULL)
	{
		char c = args[arg_it][str_it];

		while (c != '\0')
		{
			if (c == '$')
			{
				char* env_var = (char*)calloc(2, sizeof(char));
				size_t count = 1;
				c = args[arg_it][++str_it];
				if (c == '\0' || c == '$')
				{
					free(env_var);
					// $ at end of string
					// or two $ in a row
					break;
				}
				env_var[0] = c;
				env_var[1] = '\0';
				c = args[arg_it][++str_it];
				while (c != '/' && c != '\0' && c != '$')
				{
					env_var = BPushString(env_var, c);
					c = args[arg_it][++str_it];
					count++;
				}
				char* ret_env = getenv(env_var);
				if (ret_env == NULL)
				{
					free(env_var);
					// invalid env variable
					break;
				}
				args[arg_it] = CharRep(args[arg_it], str_it - count - 1, str_it - 1, ret_env);
				// must update iterator since string was changed
				str_it = str_it + strlen(env_var);
				free(env_var);
			}
			c = args[arg_it][++str_it];
		}
		str_it = 0;
		++arg_it;
	}

	return args;
}
