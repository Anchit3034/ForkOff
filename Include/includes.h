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

typedef enum{
	echo=0,
	ex_it,
	cd,
	pwd,
	type,
	de_fault
}COMMAND;

typedef enum{
	PWD,
	LOCAL_EXECUTION,
}TYPE_PWD_EXE;

typedef enum{
	normal=0,
	single_q,
	double_q,
	backslash
}QUOTING;

typedef enum{
	WRITE,
	ERROR,
	NO_ERR,
	APPEND,

}TAG_OP;

int main(void);

#endif
