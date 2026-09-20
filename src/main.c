#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>


char command[1024];

void handle_type(char const* cmd){

	if (!strcmp(cmd,"echo") || !strcmp(cmd,"exit") || !strcmp(cmd,"type")){
			printf("%s is the shell builtin\n",cmd);
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
	printf("%s is not a command\n",cmd);
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
		else if(strncmp(command,"type ",5)==0){
			handle_type(command+5);
		
		}
		else{
			printf("%s: command not found\n",command);
		}
	}
	return 0;
}
