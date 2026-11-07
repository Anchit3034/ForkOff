//#include<string.h>
#include "./../Include/buildins.h"




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

void curr_work_dir(int type, char *command){    
        char cwd[MAX_DIR_LENGTH];
        if((getcwd(cwd,sizeof(cwd))!=NULL)){
                if(type==1) printf("%s\n",cwd);
                else sprintf(buffer,"%s/%s",cwd,command);
        }
        else perror("getcwd error");
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
	int count=0;
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

void file_redir(char *filename,char *input, int type,int err_flag){
	
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

void print_echo(char *input){


	char *redirect_ptr = strchr(input,'>');
	if(redirect_ptr!=NULL){

// temp solution
		
		char *table[MAXIMUM_COMMAND_LENGTH];
		tokenisation(table,redirect_ptr+1,">");

		//tokenisation(table,redirect_ptr+1,">> ");
	
		int i=0;
		while(table[i]!=NULL){
			
			int type=0;
			int err_flag=0;

			if(strncmp(redirect_ptr-1,"2>",2)==0){
				err_flag=1;
			}

			if(strncmp(redirect_ptr,">>",2)==0){
				type = 1;
				redirect_ptr += strlen(table[i]);
			}
			trim_space(table[i]);
			file_redir(table[i++],input,type,err_flag);
			
		}
		


//temp solution
	}else{
		printer(input);

	}
	return;
		
	

}





