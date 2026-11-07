

void tokenisation(char **table,char *command,char *str){
	if(command == str){
		command++;
	}
	char *temp=strtok(command,str);

	int count=0;
	while(temp!=NULL){
		
		table[count]=temp;
		count++;
		if(count > MAXIMUM_COMMAND_LENGTH) break;

		temp=strtok(NULL,str);
		if(temp == str){
			temp++;
		}
	}
	table[count]=NULL;
}



