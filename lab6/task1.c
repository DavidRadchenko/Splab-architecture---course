#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <linux/limits.h>
#include  <sys/types.h>
#include "LineParser.h"
#include <sys/wait.h>
#include <stdio.h>

FILE *fp1, *fp2;

int dflag = 0;

void execute(cmdLine *pCmdLine) {
	int pid = 0, fPid = 0, *status = 0, a = 0;
	 
	pid = fork();
        

	if (pid == 0) {
            /*if(pCmdLine->inputRedirect != NULL)
	        fp1 = fopen(pCmdLine->inputRedirect, "r");
            
            if(pCmdLine->outputRedirect != NULL)
	        fp2 = fopen(pCmdLine->outputRedirect, "w");
            
*/
            if(pCmdLine->inputRedirect != NULL)
	        fp1 = freopen(pCmdLine->inputRedirect, "r", stdin);
	    
	    if(pCmdLine->outputRedirect != NULL)
	        fp2 = freopen(pCmdLine->outputRedirect, "w", stdout);
	    
	
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
                fPid = (pid_t)waitpid(pid, status, 0);
                /*fclose(fp1);
                fclose(fp2);
*/
                if (fPid == -1)
                    fprintf(stderr, "error");
            }
        }
    if (dflag)
        fprintf(stderr,"PID: %d, Executing command: %s", pid, pCmdLine->arguments[0]);
    
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
