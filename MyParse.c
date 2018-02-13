#include "MyParse.h"
#include "MyFunctions.h"
#include<sys/stat.h>
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
	/*
  	int wspace_count = 0;      //whitespace_count
	size_t cnt = 0;       //count iterator
	char current_char = line[cnt];   
	
	while ((current_char == '\t ' || current_char == '\n' || current_char == ' ') && (current_char != '\0'))  // delete leading whitespace
	(    
		current_char = line[++cnt];
		wspace_count++;
	)
	if (cnt > 0)    //if cnt index is more than 0
		line = DelFunc(line, cnt - wspace_count, cnt - 1);         
	
	cnt = 0;
	wspace_count = 0;
	
	//flag to tell us if there is any trailing whitespace     
	int trailing_wspace = 0;                
	
	       
	while (current_char != '\0')
	(
		current_char = line[cnt++];
		while (current_char == '\n' || current_char == '\t' || current_char == ' ')
		(
			current_char = line[cnt++];
			wspace_count++;
			if (current_char == '\0')
				trailing_wspace = 1;
		)
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
	)
	
	return line;
	*/
	
		size_t it = 0;
	int whitespace_count = 0;
	char cur_char = line[it];
	
	// delete leading whitespace
	while (cur_char != '\0' && (cur_char == ' ' || cur_char == '\t' || cur_char == '\n'))
	{
		cur_char = line[++it];
		whitespace_count++;
	}
	if (it > 0)
		line = DelFunc(line, it-whitespace_count, it-1);
	
	
	
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
	
	
	return line;
}

// Converts the line input from the user into the array of command arguments.
char** PArguments(char* input)            
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



/*
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
		
		if (cmnd_type == 3)    //if the command chosen is etime or limits
		{
			if ((strcmp(current_cmnd, "etime") == 0) || (strcmp(current_cmnd, "limits") == 0))
			{
				if (argmn_it == (cmnd_it + 1) && (CharCheck(args[argmn_it], '/') == 1))   
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
*/

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
					environment_v = BPushString(environment_v, ch);  
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
{    
	/*
	//seperates and stores the values
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
	*/
	
	
	
		input = PWhitespace(input);
	
	char** split_args = PArguments(input);
	
	// check for leading '&' and remove if present
	if (split_args[0] != NULL)
	{
		if (strcmp(split_args[0], "&") == 0)
		{
			split_args = RemoveArr(split_args, 0);
		}
	}
	
	split_args = Expand(split_args);
	
	split_args = PathResolve(split_args);
	
	// debug message
	//PrintArgVector(split_args);
	
	free(input);
	
	return split_args;
}

/*
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
*/
