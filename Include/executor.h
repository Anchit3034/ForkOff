#ifndef EXECUTE_H
#define EXECUTE_H
#include "executor.h"
#include "parser.h"
#include "buildins.h"
#include "includes.h"



void remove_quotes(char *str);
void command_runner(char *cmd);
int find_path(char *command,int type);


#endif
