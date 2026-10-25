#ifndef BUILDIN_H
#define BUILDIN_H

#include<string.h>
#include<stdint.h>
#include "executor.h"


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

COMMAND command_to_emun(char *str);

QUOTING quote_to_enum(cahr str);

void curr_work_dir(int type, char *command);
void handle_type(char *command);
char *trim_space(char *str);
void printer(char *input);
void file_redir(char *filename,char *input, int8_t type,int8_t err_flag);
void print_echo(char *input);
void curr_work_dir(int type, char *command);


#endif




