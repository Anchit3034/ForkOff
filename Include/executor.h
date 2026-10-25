#ifndef EXECUTE_H
#define EXECUTE_H

#include<stdint.h>
#include "parser.h"

void remove_quotes(char *str);
void command_runner(char *cmd);
int8_t find_path(char *command,int type);


#endif
