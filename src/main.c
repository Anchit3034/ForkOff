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
		printf("%s: command not found\n",command);
	}
	return 0;
}
