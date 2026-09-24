#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdint.h>


char command[1024];
char buffer[512];


typedef enum{
	echo=0,
	ex_it,
	cd,
	pwd,
	type,
	de_fault
}COMMAND;

COMMAND command_to_emun(char *str){
	if(strncmp(str,"echo ",5)==0) return echo;
	if(strcmp(str,"exit")==0) return ex_it;
	if(strncmp(str,"cd ",3)==0) return cd;
	if(strncmp(str,"type ",5)==0) return type;
	if(strcmp(str,"pwd")==0) return pwd;
	return de_fault;
}


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

void print_echo(char *input){
	bool is_single_quotes=false;
	uint8_t count=0;
	bool is_double_quotes=false;
	for(int i=0;input[i]!='\0';i++){
		char curr=input[i];
		if (curr=='\"'){
			is_double_quotes = !is_double_quotes;
			continue;
		}
		if (is_double_quotes){
			putchar(curr);
			continue;
		}
		if(curr=='\''){
			is_single_quotes = !is_single_quotes;
			continue;
		}
		if (is_single_quotes){
			putchar(curr);
			count=0;
		}
		else{
			if (curr!=' ' && curr!='\t'){
				putchar(curr);
				count=0;
			}
			else{
				if(count==0){
					putchar(' ');
					count=1;
				}
			}
		}
	}
	putchar('\n');

}



int main(void){

	setbuf(stdout,NULL);

	while(1){

		
		printf("$ ");
		fgets(command,sizeof(command),stdin);

		command[strcspn(command,"\n")]='\0';
		
		COMMAND cmd = command_to_emun(command);
		switch(cmd){
			case ex_it:
				goto l1;
			case echo:
				print_echo(command+5);
				break;
			case pwd:
				char cwd[1024];
				if(getcwd(cwd,sizeof(cwd))!=NULL) printf("%s\n",cwd);
				else perror("getcwd error");
				break;
			case type:
				handle_type(command+5);
				break;
			case cd:
				char *s=command+3;
				if(strcmp(s,"~")==0) s=getenv("HOME");
				if(chdir(s)==0);
				else printf("cd: %s: No such file or directory\n",s);
				break;
			default:
				char *temp=strtok(command," ");
				char *table[51];
				int count=0;
				while(temp!=NULL){
					table[count]=temp;
					count++;
					if(count >= 50) break;
					temp=strtok(NULL," ");
				}
				table[count]=NULL;

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
					printf("%s: command not found\n",command);
				}
				break;
		}


			
	}
l1:
	return 0;
}
