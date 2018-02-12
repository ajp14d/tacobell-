#include "parse.h"
#include "utility.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char* GetInput()                         
{                                            
	size_t BUFF_SIZE = 255;    //user input will be no more than 255 characters
	char* input_size = (char*)calloc(BUFF_SIZE, sizeof(char));
	if(fgets(input_size, (int)BUFF_SIZE, stdin))                                  
		return input_size;                       	                                                   
	else                                        
		return NULL;
}

char* PWhitespace(char* line)                   
{
  	int wspace_count = 0;      //whitespace_count
	size_t cnt = 0;       //count iterator
	char current_char = line[cnt];   
	
	while ((current_char == '\t ' || current_char == '\n' || current_char == ' ') && current_char != '\0')  // delete leading whitespace
	{    
		current_char = line[++cnt];
		wspace_count++;
	}
	if (cnt > 0)    //if cnt index is more than 0
		line = DelFunc(line, cnt - wspace_count, cnt - 1);         
	
	cnt = 0;
	wspace_count = 0;
	
	//flag to tell us if there is any trailing whitespace     
	int trailing_wspace = 0;                
	
	do         
	{       
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

	char* temp = strtok(input, " \n\t");    //tmp  //tokenize the input line   //////////////////////
	//char* temp; 
	//sscanf(input, "%s \n\t", temp);
	//isspace
	char** retrn = (char**)calloc(tokn_c + 1, sizeof(char*));      //allocate memory

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

char** PathResolve(char** args)           
{      //iterator, parent cmd type, iterator to point to current command, flag for new cmd (after | < or >)
	int argmn_it = 0, cmnd_type = 0, cmnd_it = 0, new_cmnd = 1;	  
	char* current_cmnd = args[0];     

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

		else if (cmnd_type == 1)     //if the commands chosen are external      
		{
			if (argmn_it == cmnd_it && (CharCheck(args[argmn_it], '/') == 1))    
				args[argmn_it] = PathMaker(args[argmn_it]);
			
			else if (argmn_it == cmnd_it)
				args[argmn_it] = PathFromEVar(args[argmn_it]);
		}
		++argmn_it;
	}while(args[argmn_it] != NULL);
	return args;
}

char** Expand(char** args)       //Expands all environment variables in the command argument array  ///
{      //argument iterator   //string iterator
	size_t argmn_it = 0, strng_it = 0;  
	do 
	{
		char ch = args[argmn_it][strng_it];
		do		
		{
			if (ch == '$')     
			{
				char* environment_v = (char*)calloc(2, sizeof(char));     
				ch = args[argmn_it][++strng_it];
				size_t counter = 1;     
				if (ch == '\0' || ch == '$')
				{
					free(environment_v);    //if there is $ at the end of the string or two $ in a row
					break;
				}
				environment_v[0] = ch;
				environment_v[1] = '\0';
				ch = args[argmn_it][++strng_it];
				while (ch != '$' && ch != '\0' && ch != '/')
				{
					environment_v = BPushString(environment_var, ch);  
					ch = args[argmn_it][++strng_it];
					counter++;
				}
				char* retrn_environment = getenv(environment_v);    
				if (retrn_environment == NULL)
				{
					free(environment_v);   // invalid environment variable
					break;
				}
				args[argmn_it] = CharRep(args[argmn_it], strng_it - counter - 1, strng_it - 1, retrn_environment);
				strng_it = strng_it + strlen(environment_v);   //update iterator since string changed  				free(environment_v);
			}
			ch = args[argmn_it][++strng_it];
		}while (ch != '\0');    //while the character doesnt equal null
		++argmn_it;
		strng_it = 0;
	}while (args[argmn_it] != NULL);
	return args;
}

char** ParseI(char* input)                          
{                                               //seperates and stores the values
	//first get rid of any whitespace
	input = PWhitespace(input);     
	char** split = PArguments(input);       //load it into the array of command arguments.
	
	if(split[0] != NULL)          //if '&' is in front remove it     
	{
		if(strcmp(split[0], "&") == 0)
			split = RemoveArr(split, 0);
	}
	
	split = Expand(split);          //Expand all environment variables in the command argument array
	split = PathResolve(split);           // Resolve all paths in the command argument array
	
	free(input);    //must free all of the memory 
	return split;      //return the newly parsed argument 
}
