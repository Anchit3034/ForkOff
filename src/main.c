#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdint.h>
#include<fcntl.h>
#include<pthread.h>
#include<signal.h>
#include<ctype.h>


//char command[1024];
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
	if(strncmp(str,"exit",4)==0) return ex_it;
	if(strncmp(str,"cd ",3)==0) return cd;
	if(strncmp(str,"type ",5)==0) return type;
	if(strncmp(str,"pwd",3)==0) return pwd;
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

int8_t find_path(char *command,int type){
	
	int8_t found=0;
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

void handle_type(char *command){
	//char *command = (char *)cmd;

	if (!strncmp(command,"echo",4) || !strncmp(command,"exit",4) || !strncmp(command,"type",4) || !strncmp(command,"pwd",3) || !strncmp(command,"cd",2)){
			printf("%s is a shell builtin\n",command);
			return;
	}
	find_path(command,1);
	return;
}

void tokenisation(char **table,char *command,char *str){
	if(command == str){
		command++;
	}
	char *temp=strtok(command,str);

	int count=0;
	while(temp!=NULL){
		
		table[count]=temp;
		count++;
		if(count >= 65536) break;

		temp=strtok(NULL,str);
		if(temp == str){
			temp++;
		}
	}
	table[count]=NULL;
}


char *trim_space(char *str){
	char *start= str;
	int read=0,write=0;
	while(start[read]!='\0'){
		if(start[read]!=' ' && start[read]!='>'){
			start[write++]=start[read];
		}
		read++;
	}
	start[write]='\0';
	return start;
}

void printer(char* input){
	bool is_single_quotes=false;
	uint8_t count=0;
	bool is_double_quotes=false;
	bool is_backslash=false;
	
	for(int i=0;input[i]!='\0' && input[i]!='>';i++){
		char curr=input[i];
		if((input[i]=='1' || input[i]=='2') && input[i+1]=='>'){
			break;
		}

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


void file_redir(char *filename,char *input, int8_t type,int8_t err_flag){
	
	//const char *restrict type = (strstr(input,">>") != NULL) ? "a":"w";
	//int8_t move = (type==(char *restrict)"a") ? 1:0;
	//char *file = trim_space(filename+move);
	//FILE *fptr=fopen(file,type);
	
	int saved_std;
	int used_std;

	if(err_flag==1){
		saved_std = dup(STDERR_FILENO);
		used_std = STDERR_FILENO;
	}
	
	else{
		saved_std = dup(STDOUT_FILENO);
		used_std = STDOUT_FILENO;
	}


	if(saved_std<0){
		perror("Failed to dup");
		return;
	}
	int fd;
	if(type==1) fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	
	else fd = open(filename,O_WRONLY | O_CREAT | O_TRUNC, 0644);


	if(fd < 0){
		perror("failed redirecting");
		return;
	}
	
	if(dup2(fd,used_std)<0){
		printf("dup2 failed to redirect");
		return;
	}

	close(fd);
	printer(input);


	fflush(stdout);
	//fclose(fptr);
	if(dup2(saved_std,used_std)<0){
		printf("dup2 failted to redir");
		return;
	}
	close(saved_std);

}

void command_runner(char *cmd){
	//char *cmd = (char*)command;
	char *table[65536];
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


void print_echo(char *input){
	//char* input = (char *)in;
	/*bool is_single_quotes=false;
	uint8_t count=0;
	bool is_double_quotes=false;
	bool is_backslash=false;*/

	char *redirect_ptr = strchr(input,'>');
	if(redirect_ptr!=NULL){

// temp solution
		
		char *table[51];
		tokenisation(table,redirect_ptr+1,">");

		//tokenisation(table,redirect_ptr+1,">> ");
	
		int8_t i=0;
		while(table[i]!=NULL){
			
			int8_t type=0;
			int8_t err_flag=0;

			if(strncmp(redirect_ptr-1,"2>",2)==0){
				err_flag=1;
			}

			if(strncmp(redirect_ptr,">>",2)==0){
				type = 1;
				redirect_ptr += strlen(table[i]);
			}
			file_redir(table[i++],input,type,err_flag);
			
		}
		


//temp solution
	}else{
		printer(input);

	}
	return;
		
	

}

void curr_work_dir(int type, char *command){
	//char *cwd=getcwd(NULL,0);
	char cwd[128];
	if((getcwd(cwd,sizeof(cwd))!=NULL)){
		if(type==1) printf("%s\n",cwd);
		else sprintf(buffer,"%s/%s",cwd,command);
	//	free(cwd);
	}
	else perror("getcwd error");
}


void remove_quotes(char *str){
	int i=0;
	int j=0;

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
	

	signal(SIGINT,SIG_IGN);
		
	

	char command[1024];
	while(1){
l2:
		printf("$ ");
		fgets(command,sizeof(command),stdin);



		if(command[0]=='\n') goto l2;

		command[strcspn(command,"\n")]='\0';
		
		COMMAND cmd = command_to_emun(command);
		switch(cmd){
			case ex_it:
				goto l1;
			case echo:
				print_echo(command+5);
				//pthread_create(&t,NULL,print_echo,command+5);
				//pthread_join(t,NULL);

				break;
			case pwd:

				curr_work_dir(1,NULL);
				break;
			case type:
				handle_type(command+5);
				//pthread_create(&t,NULL,handle_type,command+5);
				//pthread_join(t,NULL);

				break;
			case cd:
				char *s=command+3;
				if(strncmp(s,"~",1)==0) s=getenv("HOME");
				if(chdir(s)==0);
				else printf("cd: %s: No such file or directory\n",s);
				break;
			default:
				command_runner(command);

				//pthread_create(&t,NULL,command_runner,command);
				//pthread_join(t,NULL);
				break;
		}


			
	}
l1:
	return 0;
}
