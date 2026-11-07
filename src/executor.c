#include "./../Include/executor.h"

void remove_quotes(char *str){
        int i=0;
        int j=0;
        
        while(str[i]!='\0'){
                if(str[i] != '\'' && str[i] != '\"' && str[i]!='@'){
                        str[j++] = str[i];
                }
                i++;
        }
        str[j]='\0';
}

void command_runner(char *cmd){
	//char *cmd = (char*)command;
	char *table[MAXIMUM_COMMAND_LENGTH];
	tokenisation(table,cmd," ");
	if(find_path(table[0],0)){
		pid_t pid = fork();
		if(pid<0){
			perror("Forked failed\n");
			exit(1);
		}
		if(pid==0){
			execv(buffer,table);

			perror("Execv failed");
			exit(1);
		}
				
		int status;
		waitpid(pid,&status,0);					
	}
	else{
		printf("%s: command not found\n",cmd);
	}
	return;
}

int find_path(char *command,int type){
	
	int found=0;
	if(command[0]=='\'' || command[0]=='\"' || command[0]=='@'){
		remove_quotes(command);
		curr_work_dir(0,command);
		found=1;
	}
	else{

		char *PATH = strdup(getenv("PATH"));

		for(char *p=strtok(PATH,":");p!=NULL;p=strtok(NULL,":")){
			sprintf(buffer,"%s/%s",p,command);
			if(access(buffer,X_OK)==0){
				found=1;
				break;
			}
		}
		free(PATH);
		if(found && type){
			printf("%s is %s\n",command,buffer);
		}else if(type){
			printf("%s: not found\n",command);
		}
	}
	return found;
}









