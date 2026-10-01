#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdint.h>
#include<ctype.h>


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

typedef enum{
	normal=0,
	single_q,
	double_q,
	backslash
}QUOTING;



COMMAND command_to_emun(char *str){
	if(strncmp(str,"echo ",5)==0) return echo;
	if(strcmp(str,"exit")==0) return ex_it;
	if(strncmp(str,"cd ",3)==0) return cd;
	if(strncmp(str,"type ",5)==0) return type;
	if(strcmp(str,"pwd")==0) return pwd;
	return de_fault;
}

QUOTING quote_to_enum(char str){
	if(str=='\'') return single_q;
	else if(str=='\"') return double_q;
	else if(str=='\\') return backslash;
	return normal;
}


void curr_work_dir(int type, char *command);

void remove_quotes(char *str);

int find_path(char *command,int type){
	
	int found=0;
	if(command[0]=='\'' || command[0]=='\"' || command[0]=='%'){
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

void handle_type(char *cmd){
	if (!strcmp(cmd,"echo") || !strcmp(cmd,"exit") || !strcmp(cmd,"type") || !strcmp(cmd,"pwd") || !strcmp(cmd,"cd")){
			printf("%s is a shell builtin\n",cmd);
			return;
	}
	find_path(cmd,1);	
}

void trim_space(char *str){
	char *start = str;
	while(isspace((unsigned char)*start)) start++;
	if(start!=str) memmove(str,start,strlen(start)+1);

	int len=strlen(str);
	while(len>0 && isspace((unsigned char)str[len-1])){
		str[len-1]='\0';
		len--;
	}
}

void file_redir(char *filename,char *input){
	

	trim_space(filename);
	FILE *fptr = fopen(filename,"a");

	if(fptr==NULL){
		perror("error redirecting stdout");
		return;
	}
	int8_t ch=0;
	while(input[ch]!='>'){
		fputc(input[ch],fptr);
		ch++;
	}
	fclose(fptr);
}

void print_echo(char *input){
	bool is_single_quotes=false;
	uint8_t count=0;
	bool is_double_quotes=false;
	bool is_backslash=false;

	char *redirect_ptr = strchr(input,'>');
	if(redirect_ptr != NULL){

		file_redir(redirect_ptr+1,input);

	}else{

		for(int i=0;input[i]!='\0';i++){
			char curr=input[i];
			if((is_single_quotes && curr!='\'') || (is_double_quotes && curr!='\"') || (is_backslash )){
				putchar(curr);
				is_backslash=false;
				continue;
			}
			QUOTING quote = quote_to_enum(curr);

			switch(quote){
				case single_q:
					is_single_quotes = !is_single_quotes;
					break;
				case double_q:
					is_double_quotes = !is_double_quotes;
					break;
				case backslash:
					is_backslash=true;
					break;
				default:
					if (curr!=' ' && curr!='\t'){
						putchar(curr);
						count=0;
					}else{
						if(count==0){
							putchar(' ');
							count=1;
						}
					}
					break;
			}

		}
		putchar('\n');

	}

		
	

}

void curr_work_dir(int type, char *command){
	char cwd[256];
	if((getcwd(cwd,sizeof(cwd))!=NULL) && type==1) printf("%s\n",cwd);
	else if(type==0) sprintf(buffer,"%s/%s",cwd,command);
	else perror("getcwd error");
}


void remove_quotes(char *str){
	uint8_t i=0;
	uint8_t j=0;

	while(str[i]!='\0'){
		if(str[i] != '\'' && str[i] != '\"' && str[i]!='%'){
			str[j++] = str[i];
		}
		i++;
	}
	str[j]='\0';
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
				curr_work_dir(0,NULL);
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
