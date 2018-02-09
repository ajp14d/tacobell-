
void pipe(char * args[]){
	int file1[2];
	int file2[2]; 
	
	int cmd = 0; 
	char *doThis[300]; 
	
	pid_t pid; 
	
	int error = -1; 
	int done = 0; 
	
	int a = 0, int b = 0, int c = 0, int d = 0; 
	
	//we use pipe to separate commands so we need to add a counter for commands
	while(args[1] != NULL){
		if(strcmp(args[1], "|") == 0){
			cmd++; 
		}
		++1; 
	}
	cmd++; 
	
	while(args[b] != NULL && done !=1){
		c = 0; 
		
		while(strcmp(args[b], "|") != 0){
			doThis[c] = args[b]; 
			b++; 
			if(args[b] == NULL){
				done = 1; 
				c++; 
				break; 
			}
			c++; 
		}
		doThis[c] = NULL; 
		b++; 
		
		if(a % 2 != 0){
			pipe(file1); 
		}
		else
			pipe(file2); 
		
		pid = fork(); 
		
		if(pid == -1){
			if(a != cmd -1){
				if(a % 2 != 0){
					close(file1[1]); 
				}
				else
					close(file2[1]); 
			}
			printf("error with the child process\n"); 
			return; 
		}
		
	if(pid == 0){
		if(a == 0){
			dup2(file2[1], STDOUT_FILENO); 
		}
		else if(a == cmd -1){
			if(cmd % 2 != 0){
				dup2(file1[0], STDOUT_FILENO); 
			}
			else{
				dup2(file2[0], STDOUT_FILENO); 
			}
		}
		else{
			if(a % 2 != 0){
				dup2(file2[0], STDOUT_FILENO); 
				dup2(file1[1], STDOUT_FILENO); 
			}
			else{
				dup2(file1[0], STDOUT_FILENO); 
				dup2(file2[1], STDOUT_FILENO); 
			}
		}
		
		if(execvp(doThis[0], doThis) == error){
			kill(getpid(), SIGTERM); 
		}
	}
	
	if(a == 0){
		close(file2[1]); 
	}
	else if ( a == cmd - 1){
		if(cmd % 2 != 0){
			close(file1[0]); 
		}
		else{
			close(file2[0]); 
		}
	}
	else{
		if(a % 2 != 0){
			close(file2[0]); 
			close(file1[1]); 
		}
		else{
			close(file1[0]); 
			close(file2[1]); 
		}
	}
	
	waitpid(pid, NULL, 0); 
	
	i++; 
	}
}
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				