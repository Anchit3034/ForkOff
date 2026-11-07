#ifndef INCLUDE_H
#define INCLUDE_H


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdint.h>
#include<fcntl.h>
#include<signal.h>
#include<ctype.h>

#define MAXIMUM_COMMAND_LENGTH 8192
#define MAX_BUFFER_LENGTH 512
#define MAX_DIR_LENGTH 128
extern char buffer[MAX_BUFFER_LENGTH];


int main(void);

#endif
