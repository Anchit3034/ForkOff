
#include "./../Include/buildins.h"
#include "./../Include/executor.h"
#include<string.h>

#define MAXIMUM_COMMAND_LENGTH 8192
#define MAX_BUFFER_LENGTH 512
#define MAX_DIR_LENGTH 128


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





