#include <stdlib.h>
#include <stdio.h>
//Generate null-terminated array of arguments (char*)
char **parse_cmdline(char *cmdline) {
	int arglen = 8;
	int argscount = 5;
	char* argument = malloc(sizeof(char) * arglen);
	char** arguments = malloc(sizeof(char*) * argscount);
	int j=0;
	int k=0;
	for(int i=0; ; i++) {
		if(j >= arglen) {
			arglen *= 2;
			//printf("Reallocing argument\n");
			argument = realloc(argument, arglen);
		}
		if(cmdline[i] == ' ' || cmdline[i] == '\n') {
			argument[j] = '\0';
			if(k >= argscount) {
				argscount *= 2;
				arguments = realloc(arguments, argscount * sizeof(char*));
			}
			arguments[k] = argument;
			k++;
			if(cmdline[i] != '\n') {
				arglen = 8;
				argument = malloc(sizeof(char) * arglen);
				j=0;
			} 
			else {
				break;
			}	
		} else {
			argument[j] = cmdline[i];
			j++;
		}
	}
	if(k >= argscount) {
		argscount += (argscount - k + 1);
		arguments = realloc(arguments, sizeof(char*) * argscount);
	}
	arguments[k] = NULL;
	return arguments;
}

void complex_free(char*** tofree) {
	for(int i=0; (*tofree)[i] != NULL; i++) {
		free((*tofree)[i]);
	}
	free(*tofree);
}
