#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>

char command[1024];


void handle_type(char const* cmd){

	if (!strcmp(cmd,"echo") || !strcmp(cmd,"exit") || !strcmp(cmd,"type")){
			printf("%s is a shell builtin\n",cmd);
			return;
	}
	
	char *PATH=strdup(getenv("PATH"));
	for(char *p=strtok(PATH,":");p!=NULL;p=strtok(NULL,":")){
		char fp[1028];
		sprintf(fp,"%s/%s",p,cmd);
		if(access(fp,X_OK)==0){
			printf("%s is %s\n",cmd,fp);
			return;
		}
	}
	printf("%s: not found\n",cmd);
}



int main(void){

	setbuf(stdout,NULL);

	while(1){
l1:
		
		printf("$ ");
		fgets(command,sizeof(command),stdin);

		command[strcspn(command,"\n")]='\0';
		
		if(strcmp(command,"exit")==0){
			break;
		
		}else if(strncmp(command,"echo ",5)==0){
			printf("%s\n",command+5);
		}
		else if(strncmp(command,"type ",5)==0){
			handle_type(command+5);
		
		}
		else{

			char *PATH=strdup(getenv("PATH"));
			for(char *p=strtok(PATH,":");p!=NULL;p=strtok(NULL,":")){
				char fp[1028];
				sprintf(fp,"%s/%s",p,command);
				if(access(fp,X_OK)==0){
					pid_t pid = fork();
					if(pid<0){
						perror("Fork failed");
						return 1;
					}
					if(pid==0){
						
						char *args[]={command,NULL};
						
						execv(fp,args);

						perror("Exev failed");
						exit(1);
					}
					int status;
					waitpid(pid,&status,0);

						
					goto l1;
				}
			}
			printf("%s: command not found\n",command);
		}
	}
	return 0;
}
