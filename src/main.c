#include<stdio.h>
#include<stdlib.h>
#include<string.h>


char command[1024];
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
			if(!strcmp(command+5,"echo") || !strcmp(command+5,"exit") || !strcmp(command+5,"type")){
				printf("%s: is the inbuilt command in shell\n",command+5);
			}else{
				printf("%s: invalid command\n",command+5);
			}
		}
		else{
			printf("%s: command not found\n",command);
		}
	}
	return 0;
}
