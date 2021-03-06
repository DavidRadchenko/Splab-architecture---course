#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <linux/limits.h>
#include  <sys/types.h>
#include "LineParser.h"



int dflag = 0;

void execute(cmdLine *pCmdLine, char dir[]) {
	int cPid;
	cPid = fork();
	if (!cPid) {
		int ret = execvp(pCmdLine->arguments[0], pCmdLine->arguments);
		if (ret < 0) {
			perror("ERROR");
		    exit(0);
		}
	}
	if (dflag)
	  	fprintf(stderr,"PID: %d, Executing command: %s\n", cPid, pCmdLine->arguments[0]);
}

int main(int argc, char **argv) {
    char buff[PATH_MAX + 1];
    char input[2048];
    cmdLine *cmdl;
    int i = 0;
    for(i = 1; i < argc; i++) 
        if(argv[i][0] == '-' && argv[i][1] == 'd') 
            dflag = 1;
            
        
    while(1) {
    	getcwd( buff, PATH_MAX + 1 );
	    if( buff != NULL ) {
	        printf("DIR:%s\n", buff);
	    }
		if( fgets (input, 2048, stdin) != NULL ) {

			cmdl = (struct cmdLine*)parseCmdLines(input);
			if(strcmp(input, "quit\n") == 0) {
				return 0;
			}
			execute(cmdl, buff);
			freeCmdLines(cmdl);
		}
    }
    
	return 0;
}
