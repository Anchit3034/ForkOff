#ifndef BUILDIN_H
#define BUILDIN_H


#include "buildins.h"
#include "includes.h"
#include "parser.h"
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

QUOTING quote_to_enum(char str);

void curr_work_dir(int type, char *command);
void handle_type(char *command);
char *trim_space(char *str);
void printer(char *input);
void file_redir(char *filename,char *input, int type,int err_flag);
void print_echo(char *input);


#endif




