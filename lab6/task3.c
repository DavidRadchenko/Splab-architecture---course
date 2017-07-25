#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <linux/limits.h>
#include  <sys/types.h>
#include "LineParser.h"
#include <sys/wait.h>


int dflag = 0;
int fds[2];
FILE *fp1, *fp2;

void execute(cmdLine *pCmdLine) {
	int cpid = 0, fpid = 0, *status = 0, a = 0;

    a = pipe(fds);
    if(a == -1) {
        perror("pipe");
        exit(1);
    }

	cpid = fork();
   
	if (cpid == 0) {
            if(pCmdLine->inputRedirect != NULL)
	        fp1 = freopen(pCmdLine->inputRedirect, "r", stdin);
	    
	    if(pCmdLine->outputRedirect != NULL)
	        fp2 = freopen(pCmdLine->outputRedirect, "w", stdout);
	    
            close(1);
            dup(fds[1]);
            close(fds[1]);
            a = execvp(pCmdLine->arguments[0], pCmdLine->arguments);
            if (a == -1) {
                fprintf(stderr,"exec failed\n"); 
                freeCmdLines(pCmdLine);
                exit(0);
            }
	}
    else {
        close(fds[1]); 
        fpid = fork();
        if(fpid == 0) {
            close(0);
            dup(fds[0]);
            close(fds[0]);
            a = execvp(pCmdLine->next->arguments[0], pCmdLine->next->arguments);
            if (a == -1) {
                fprintf(stderr,"exec failed\n"); 
                freeCmdLines(pCmdLine);
                exit(0);
            }      
        }
         else {
            close(fds[0]);
            waitpid(cpid, status, 0);
            waitpid(fpid, status, 0);
        }
    }

    if (dflag)
        fprintf(stderr,"PID: %d, Executing command: %s", cpid, pCmdLine->arguments[0]);
    
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
