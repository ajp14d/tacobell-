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
	char** split_arg = PArguments(input);            //split_args
	
	if(split_arg[0] != NULL)          //if '&' is in front remove it         //check for leading '&' and remove if present
	{
		if(strcmp(split_arg[0], "&") == 0)
			split_arg = RemoveArr(split_arg, 0);
	}
	
	split_arg = Expand(split_arg);           //ExpandVariables
	split_arg = PathResolve(split_arg);             //ResolvePaths
	
	// debug message
	//DisplayArgs(char** args);      //PrintArgVector(split_arg);        //////////////////
  
	free(input);    //free the input 
	return split_arg;      //return the newly parsed argument 
}

char* PWhitespace(char* line)                    //ParseWhitespace
{
  	int wspace_count = 0;      //whitespace_count
	size_t cnt = 0;       //count iterator
	char current_char = line[cnt];     //cur_char
	
	while (current_char != '\0' && (current_char == '\t ' || current_char == '\n' || current_char == ' '))  // delete leading whitespace
	{    //while the current char isnt null, a tab, a newline, or a space
		current_char = line[++cnt];
		wspace_count++;
	}
	if (cnt > 0)    //if cnt index is more than 0
		line = DelFunc(line, cnt - wspace_count, cnt - 1);              ////////////////////////////////////////
	
	// check for empty string
	//if (strcmp(line,"") == 0)
	//	return line;
	// debug message
	/*printf("Made it through leading whitespace and deleted %i characters...\n", wspace_count);
	printf(line);
	printf("\n");*/
	
	cnt = 0;
	wspace_count = 0;
	
	//flag to tell us if there is any trailing whitespace      //need flag to determine if currently operating on trailing whitespace
	int trailing_wspace = 0;                //contains_trailing
	
	while (current_char != '\0')     //delete intermediate extra whitespace
	{        //while the current char isnt being null terminated 
		current_char = line[cnt++];
		while (current_char == '\n' || current_char == '\t' || current_char == ' ')
		{
			current_char = line[cnt++];
			wspace_count++;
			if (current_char == '\0')
				trailing_wspace = 1;
		}
		if (trailing_wspace == 1)   //if there is trailing whitespace
		{
			if (wspace_count > 0)   //if that whitespace is greater than 0
				line = DelFunc(line, cnt - wspace_count - 1, cnt - 2);     ///////////////////////////////
			break;
		}
		else if (wspace_count > 1)
		{
			line = DelFunc(line, cnt - wspace_count - 1, cnt - 3);
			cnt = cnt - (wspace_count - 1);   //update the count iterator if the array is changed through deletion
		}
		wspace_count = 0;   //set white space counter back to 0
	}
	
	// debug message
	/*printf("Made it through intermediate whitespace...\n", wspace_count);
	printf(line);
	printf("%i",strlen(line));
	printf("\n");*/
	
	// bug on return                 ////////////////////////////////////////////////////////////////////////////////
	// current guess is somewhere writing over the return address
	// (buffer overflow)
	return line;
}

char** PArguments(char* input)               //ParseArguments
{
	size_t cnt = 0;
	char c = input[cnt];
	int tokn_c = 1;                 //token_count

	while (c != '\0')
	{
		if ((c == '\n ') || (c == ' ') || (c == '\t'))
			tokn_c++;
		c = input[++cnt];
	}

	int i = 0;       
	char** retrn = (char**)calloc(tokn_c + 1, sizeof(char*));    //char** ret
	char* tmp = strtok(input, " \n\t");

	if (tmp != NULL)
	{
		retrn[i] = (char*)calloc(strlen(tmp) + 1, sizeof(char));
		strcpy(retrn[i], tmp);
	}
	for (i = 1; i < tokn_c; i++)
	{
		tmp = strtok(NULL, " \n\t");
		retrn[i] = (char*)calloc(strlen(tmp) + 1, sizeof(char));
		strcpy(retrn[i], tmp);
	}
	retrn[i] = NULL;
	return retrn;
}

char** PathResolve(char** args)           //ResolvePaths
{      //iterator, parent cmd type, iterator to point to current command, flag for new cmd (after | < or >)
	int arg_it = 0, cmnd_type = 0, cmnd_it = 0, new_cmnd = 1;	    //cmd_type, cmd_int, new_cmd 
	//int cur_type;		// current type of args[arg_it]				[FIXED COMPILER WARNING UNUSED VAR]	
	char* current_cmnd = args[0];     //cur_cmd

	while(args[arg_it] != NULL)
	{
		//cur_type = IsCommand(args,arg_it);						[FIXED COMPILER WARNING UNUSED VAR]
		//printf("%s	%i\n", args[arg_it], cur_type);  
		if (new_cmnd == 1)
		{
			cmnd_type = CmdCheck(args, arg_it);  ////////////////////
			current_cmnd = args[arg_it];
			cmnd_it = arg_it;
			new_cmnd = 0;
		}
		else
		{
			if ((strcmp(args[arg_it], "<") == 0) || (strcmp(args[arg_it], ">") == 0) || (strcmp(args[arg_it], "|") == 0))
			{
				new_cmnd = 1;
				++arg_it;
				continue;
			}
		}
		
		// cd
		if (cmnd_type == 2)    //if the command is cd
		{
			if (arg_it == (cmnd_it + 1))
			{
				if (!CharCheck(args[arg_it], '/'))   /////////////////////////////////////
				{
					if (!CharCheck(args[arg_it], '~') && !CharCheck(args[arg_it], '.'))//////////////////////////
					{
						args[arg_it] = FPushString(args[arg_it], '/');/////////////////
						args[arg_it] = FPushString(args[arg_it], '.');/////////////////
					}
				}
				else
				{
					if (args[arg_it][0] != '.' && args[arg_it][0] != '~' && args[arg_it][0] != '/')
					{
						args[arg_it] = FPushString(args[arg_it], '/');///////////////////
						args[arg_it] = FPushString(args[arg_it], '.');///////////////////
					}
				}
				args[arg_it] = PathMaker(args[arg_it]);     ////////////////////////
			}
		}

		// etime and limits
		else if (cmnd_type == 3)    //if the command chosen is etime or limits
		{
			if ((strcmp(current_cmnd, "etime") == 0) || (strcmp(current_cmnd, "limits") == 0))
			{
				if (arg_it == (cmnd_it + 1))
				{
					if (CharCheck(args[arg_it], '/') == 1)///////////////////////////
						args[arg_it] = PathMaker(args[arg_it]);//////////////////
					else
						args[arg_it] = PathFromEVar(args[arg_it]);///////////////////

					//args[arg_it] = BuildPath(args[arg_it]);
				}
			}
		}

		// external commands
		else if (cmnd_type == 1)     //if the commands chosen are external
		{
			if (arg_it == cmnd_it)
			{
				if (CharCheck(args[arg_it], '/') == 1)//////////////////
					args[arg_it] = PathMaker(args[arg_it]);//////////////////
				else
					args[arg_it] = PathFromEVar(args[arg_it]);///////////////////////
			}
		}
		++arg_it;
	}
	return args;
}

char** Expand(char** args)       //ExpandVariables
{
	size_t arg_it = 0;    //argument iterator
	size_t str_it = 0;     //string iterator
	//char* str = args[arg_it];
	while (args[arg_it] != NULL)
	{
		char c = args[arg_it][str_it];
		while (c != '\0')    //while the character doesnt equal null
		{
			if (c == '$')     
			{
				char* environment_v = (char*)calloc(2, sizeof(char));     //env_var
				size_t counter = 1;     //count
				c = args[arg_it][++str_it];
				if (c == '\0' || c == '$')
				{
					free(environment_v);    //$ at end of string or two $ in a row
					break;
				}
				environment_v[0] = c;
				environment_v[1] = '\0';
				c = args[arg_it][++str_it];
				while (c != '$' && c != '\0' && c != '/')
				{
					environment_v = BPushString(environment_var, c);   /////////////////
					c = args[arg_it][++str_it];
					counter++;
				}
				char* retrn_environment = getenv(environment_v);    //ret_env  //////////////////////////getenv(env_var)
				if (retrn_environment == NULL)
				{
					free(environment_v);   // invalid env variable
					break;
				}
				args[arg_it] = CharRep(args[arg_it], str_it - counter - 1, str_it - 1, retrn_environment);////////////
				str_it = str_it + strlen(environment_v);   //update iterator since string changed  //must update iterator since string was changed
				free(environment_v);
			}
			c = args[arg_it][++str_it];
		}
		str_it = 0;
		++arg_it;
	}
	return args;
}
