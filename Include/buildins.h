#ifndef BUILDIN_H
#define BUILDIN_H


#include "buildins.h"
#include "includes.h"
#include "parser.h"
#include "executor.h"



COMMAND command_to_emun(char *str);

QUOTING quote_to_enum(char str);

void curr_work_dir(int type, char *command);
void handle_type(char *command);
char *trim_space(char *str);
void printer(char *input);
void file_redir(char *filename,char *input, TAG_OP type,TAG_OP err_flag);
void print_echo(char *input);


#endif




