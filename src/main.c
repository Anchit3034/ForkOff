#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>


char command[1024];
char buffer[512];




int find_path(char *command,int type){
	char *PATH = strdup(getenv("PATH"));
	int found=0;

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
	return found;
}
void handle_type(char *cmd){

	if (!strcmp(cmd,"echo") || !strcmp(cmd,"exit") || !strcmp(cmd,"type") || !strcmp(cmd,"pwd") || !strcmp(cmd,"cd")){
			printf("%s is a shell builtin\n",cmd);
			return;
	}
	find_path(cmd,1);	
}



int main(void){

	setbuf(stdout,NULL);

	while(1){

		
		printf("$ ");
		fgets(command,sizeof(command),stdin);

		command[strcspn(command,"\n")]='\0';
		
		if(strcmp(command,"exit")==0){
			break;
		
		}else if(strncmp(command,"echo ",5)==0){
			printf("%s\n",command+5);
		}
		else if(strcmp(command,"pwd")==0){
			char cwd[1024];
			if(getcwd(cwd,sizeof(cwd))!=NULL){
				printf("%s\n",cwd);

			}
			else{
				perror("Getcwd() error");
			}
		}
		else if(strncmp(command,"type ",5)==0){
			handle_type(command+5);
		
		}
		else if(strncmp(command,"cd ",3)==0){
			char *s=command+3;
			if(chdir(s)==0);
			else{
				printf("cd: %s: No such file or directory\n",s);
			}
		}

		else{

			char *temp=strtok(command," ");
			char *table[51];
			int count=0;
			while(temp!=NULL){
				table[count]=temp;
				count++;
				if(count >= 50){
					break;
				}
				temp=strtok(NULL," ");
			}
			table[count]=NULL;

			if(find_path(table[0],0)){
				pid_t pid = fork();
				if(pid<0){
					perror("Fork failed");
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
				printf("%s: command not found\n",command);
			}		
		}			
	}
	return 0;
}
