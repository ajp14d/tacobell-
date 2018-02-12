#include "parse.h"
#include "utility.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char* GetInput()                         //char* ReadInput()
{                                              //gets whatever input the user puts in
	size_t BUFF_SIZE = 255;    //user input will be no more than 255 characters
	char* input_size = (char*)calloc(BUFF_SIZE, sizeof(char));
	if(fgets(input_size, (int)BUFF_SIZE, stdin))                                  
		return input_size;                       	                                                   
	else                                        
		return NULL;
}

char* PWhitespace(char* line)                    //ParseWhitespace
{
  	int wspace_count = 0;      //whitespace_count
	size_t cnt = 0;       //count iterator
	char current_char = line[cnt];     //cur_char
	
	while ((current_char == '\t ' || current_char == '\n' || current_char == ' ') && current_char != '\0')  // delete leading whitespace
	{    
		current_char = line[++cnt];
		wspace_count++;
	}
	if (cnt > 0)    //if cnt index is more than 0
		line = DelFunc(line, cnt - wspace_count, cnt - 1);         
	
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
	
	do          //delete intermediate extra whitespace
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
				line = DelFunc(line, cnt - wspace_count - 1, cnt - 2);  //delete the whitespace   
			break;
		}
		else if (wspace_count > 1)
		{
			line = DelFunc(line, cnt - wspace_count - 1, cnt - 3);   //delete the whitespace 
			cnt = cnt - (wspace_count - 1);   //update the count iterator if the array is changed through deletion
		}
		wspace_count = 0;   //set white space counter back to 0
	}while (current_char != '\0');
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

// Converts the line input from the user into the array of command arguments.
char** PArguments(char* input)               //ParseArguments
{
	size_t cnt = 0;
	char ch = input[cnt];
	int tokn_c = 1, i = 0;                 //token_count  

	while (ch != '\0')
	{
		if ((ch == '\n') || (ch == ' ') || (ch == '\t'))
			tokn_c++;
		ch = input[++cnt];
	}

	char* temp = strtok(input, " \n\t");    //tmp  //tokenize the input line   ///////////////////////////////////////////
	//char* temp; 
	//sscanf(input, "%s \n\t", temp);
	//isspace
	char** retrn = (char**)calloc(tokn_c + 1, sizeof(char*));    //char** ret  //allocate memory

	if (temp != NULL)
	{
		retrn[i] = (char*)calloc(strlen(temp) + 1, sizeof(char));
		strcpy(retrn[i], temp);
	}
	for (i = 1; i < tokn_c; i++)
	{
		temp = strtok(NULL, " \n\t");///////////////////////////////////////////////////////////////
		//sscanf(NULL, "%s \n\t", temp);
		retrn[i] = (char*)calloc(strlen(temp) + 1, sizeof(char));
		strcpy(retrn[i], temp);
	}
	retrn[i] = NULL;
	return retrn;
}

char** PathResolve(char** args)           //ResolvePaths                    //check if cmmnd count is correct
{      //iterator, parent cmd type, iterator to point to current command, flag for new cmd (after | < or >)
	int argmn_it = 0, cmnd_type = 0, cmnd_it = 0, new_cmnd = 1;	    //cmd_type, cmd_int, new_cmd 
	//int cur_type;		// current type of args[arg_it]				[FIXED COMPILER WARNING UNUSED VAR]	
	char* current_cmnd = args[0];     //cur_cmd

	do 
	{          // cd
		if (cmnd_type == 1)    //if the command is cd
		{
			if (argmn_it == (cmnd_it + 1))
			{
				if (!CharCheck(args[argmn_it], '/') && !CharCheck(args[argmn_it], '~') && !CharCheck(args[argmn_it], '.'))   
				{
					args[argmn_it] = FPushString(args[argmn_it], '/');
					args[argmn_it] = FPushString(args[argmn_it], '.');
				}
				else if (args[argmn_it][0] != '.' && args[argmn_it][0] != '~' && args[argmn_it][0] != '/')
				{
					args[argmn_it] = FPushString(args[argmn_it], '/');
					args[argmn_it] = FPushString(args[argmn_it], '.');
				}
				args[argmn_it] = PathMaker(args[argmn_it]);   
			}
		}
		
		//cur_type = IsCommand(args,arg_it);						[FIXED COMPILER WARNING UNUSED VAR]
		//printf("%s	%i\n", args[arg_it], cur_type);  
		else if (new_cmnd == 2)
		{
			cmnd_type = CmdCheck(args, argmn_it); 
			current_cmnd = args[argmn_it];
			cmnd_it = argmn_it;
			new_cmnd = 0;
		}
		else                                        
		{
			if ((strcmp(args[argmn_it], "<") == 0) || (strcmp(args[argmn_it], ">") == 0) || (strcmp(args[argmn_it], "|") == 0))
			{
				new_cmnd = 1;
				++argmn_it;
				continue;
			}
		}
		
		// etime and limits
		else if (cmnd_type == 3)    //if the command chosen is etime or limits
		{
			if ((strcmp(current_cmnd, "etime") == 0) || (strcmp(current_cmnd, "limits") == 0))
			{
				if (argmn_it == (cmnd_it + 1) && (CharCheck(args[argmn_it], '/') == 1))   //////
					args[argmn_it] = PathMaker(args[argmn_it]);
				
				else if (argmn_it == (cmnd_it + 1))
					 args[argmn_it] = PathFromEVar(args[argmn_it]);
			}
		}

		// external commands
		else if (cmnd_type == 1)     //if the commands chosen are external       /////
		{
			if (argmn_it == cmnd_it && (CharCheck(args[argmn_it], '/') == 1))     ///
				args[argmn_it] = PathMaker(args[argmn_it]);
			
			else if (argmn_it == cmnd_it)
				args[argmn_it] = PathFromEVar(args[argmn_it]);
		}
		++argmn_it;
	}while(args[argmn_it] != NULL);
	return args;
}

char** Expand(char** args)       //ExpandVariables   //Expands all environment variables in the command argument array
{
	size_t argmn_it = 0;    //argument iterator
	size_t strng_it = 0;     //string iterator
	//char* str = args[arg_it];
	do 
	{
		char c = args[argmn_it][strng_it];
		while (c != '\0')    //while the character doesnt equal null
		{
			if (c == '$')     
			{
				char* environment_v = (char*)calloc(2, sizeof(char));     //env_var
				c = args[argmn_it][++strng_it];
				size_t counter = 1;     //count
				if (c == '\0' || c == '$')
				{
					free(environment_v);    //if there is $ at the end of the string or two $ in a row
					break;
				}
				environment_v[0] = c;
				environment_v[1] = '\0';
				c = args[argmn_it][++strng_it];
				while (c != '$' && c != '\0' && c != '/')
				{
					environment_v = BPushString(environment_var, c);  
					c = args[argmn_it][++strng_it];
					counter++;
				}
				char* retrn_environment = getenv(environment_v);    //ret_env  getenv(env_var)
				if (retrn_environment == NULL)
				{
					free(environment_v);   // invalid env variable
					break;
				}
				args[argmn_it] = CharRep(args[argmn_it], strng_it - counter - 1, strng_it - 1, retrn_environment);
				strng_it = strng_it + strlen(environment_v);   //update iterator since string changed  				free(environment_v);
			}
			c = args[argmn_it][++strng_it];
		}
		++argmn_it;
		strng_it = 0;
	}while (args[argmn_it] != NULL);
	return args;
}

char** ParseI(char* input)                           //ParseInput
{                                               //seperates and stores the values
	//first get rid of any whitespace
	input = PWhitespace(input);       //ParseWhitespace
	char** split = PArguments(input);      //split_args   //load it into the array of command arguments.
	
	if(split[0] != NULL)          //if '&' is in front remove it         //check for leading '&' and remove if present
	{
		if(strcmp(split[0], "&") == 0)
			split = RemoveArr(split, 0);
	}
	
	split = Expand(split);           //ExpandVariables  //Expand all environment variables in the command argument array
	split = PathResolve(split);             //ResolvePaths  // Resolve all paths in the command argument array
	
	// debug message
	//DisplayArgs(char** args);      //PrintArgVector(split_arg);       
  
	free(input);    //must free all of the memory 
	return split;      //return the newly parsed argument 
}
