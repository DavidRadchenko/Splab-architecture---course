#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <linux/limits.h>
#include  <sys/types.h>
#include "LineParser.h"
#include <sys/wait.h>


int dflag = 0;

void execute(cmdLine *pCmdLine) {
	int cPid = 0, fPid = 0, *status = 0, a = 0;
	
	cPid = fork();
	if (cPid == 0) {
		a = execvp(pCmdLine->arguments[0], pCmdLine->arguments);
		if (a == -1) {
		  	fprintf(stderr,"exec failed\n"); 
                freeCmdLines(pCmdLine);
		  	exit(0);
		}
	}
    else {
        a = pCmdLine->blocking;
        if(a) {
            fPid = waitpid(cPid, status, 0);
            if (fPid == -1)
                fprintf(stderr, "error");
        }
    }
    if (dflag)
        fprintf(stderr,"PID: %d, Executing command: %s", cPid, pCmdLine->arguments[0]);
    
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
	    if(buff != NULL) 
	        printf("DIR:%s\n", buff);
		fgets(input, 2048, stdin);
            cmdl = parseCmdLines (input);
            if (strcmp(input, "quit\n") == 0) {
                freeCmdLines(cmdl);
                exit(0);
		}
		else if (strcmp(cmdl->arguments[0], "cd") == 0){
			int ret = chdir(cmdl->arguments[1]);
		    if (ret < 0)
		    	fprintf(stderr, "cd failed");
		} 
		else
			execute (cmdl);
                
        freeCmdLines(cmdl);
        
    }
	return 0;
}
