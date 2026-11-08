

#include "./../Include/parser.h"
#include "./../Include/executor.h"
#include "./../Include/buildins.h"
char buffer[MAX_BUFFER_LENGTH];


int main(void){


	setbuf(stdout,NULL);
	

	signal(SIGINT,SIG_IGN);
		
	

	char command[MAXIMUM_COMMAND_LENGTH];
	while(1){
__NO_INPUT__:
		printf("$ ");

		if(fgets(command,sizeof(command),stdin)==NULL) goto __EXIT_SHELL__;




		if(command[0]=='\n') goto __NO_INPUT__;

		command[strcspn(command,"\n")]='\0';
		
		COMMAND cmd = command_to_emun(command);
		switch(cmd){
			case ex_it:
				goto __EXIT_SHELL__;
			case echo:
				print_echo(command+5);

				break;
			case pwd:

				curr_work_dir(1,NULL);
				break;
			case type:
				handle_type(command+5);

				break;
			case cd:
				char *s=command+3;
				if(strncmp(s,"~",1)==0) s=getenv("HOME");
				if(chdir(s)==0);
				else printf("cd: %s: No such file or directory\n",s);
				break;
			default:
				command_runner(command);

				break;
		}


			
	}
__EXIT_SHELL__:
	return 0;
}
