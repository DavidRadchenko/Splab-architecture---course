#include "LineParser.h"
#include "JobControl.h"
#include "linux/limits.h"
#include <unistd.h>
#include "stdio.h"
#include "string.h"
#include "signal.h"
#include <sys/wait.h>
#include "termios.h"
#include <stdlib.h>
#include <sys/types.h>

#define DONE -1
#define RUNNING 1
#define SUSPENDED 0

int dflag = 0, shell_terminal;

job* newJob;
job** job_list;




void handleSignals(int signalNum){
    char *sigName = (char*)strsignal(signalNum);
    printf("Signal: %s was ignored\n", sigName);
}


void execute(cmdLine *pCmdLine) {
    pid_t cPid, fPid;
    int *status = 0, a = 0;
    cPid = fork();
    if (cPid == 0) {
            a = execvp(pCmdLine->arguments[0], pCmdLine->arguments);
            /*setting the signals back to default*/
            signal(SIGTTIN,SIG_DFL);
            signal(SIGTTOU,SIG_DFL);
            signal(SIGQUIT, handleSignals);
            signal(SIGCHLD, handleSignals);
            signal(SIGTSTP,SIG_DFL);
            
             /*Set the process group id of the new process to be the same as the child process id*/
            setpgid(newJob->pgid,newJob->pgid);
        
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
    job_list = malloc(sizeof(job));
    shell_terminal = STDIN_FILENO;
    setpgid(getpgid(),getpgid());


    signal (SIGQUIT, handleSignals);
    signal (SIGCHLD, handleSignals);
    signal(SIGTTIN,SIG_IGN);
    signal(SIGTTOU,SIG_IGN);
    signal(SIGTSTP,SIG_IGN);

            

    while(1) {
           
                getcwd( buff, PATH_MAX + 1 );
                if(buff != NULL) 
                    fgets(input, 2048, stdin);
                cmdl = parseCmdLines(input);

                if(strcmp(input,"jobs\n") == 0){
                    printJobs(job_list);
                    continue;
                }
                
                newJob = addJob(job_list, input);
                newJob->status = RUNNING;
                
                
                if (strcmp(input, "quit\n") == 0) {
                    freeCmdLines(cmdl);
                    freeJobList(job_list);
                    exit(0);
		}
		else if (strcmp(cmdl->arguments[0], "cd") == 0){
                    int ret = chdir(cmdl->arguments[1]);
		    if (ret < 0)
		    	fprintf(stderr, "cd failed");
		} 
		execute(cmdl);
                freeCmdLines(cmdl);
                
		
    }
	return 0;
}
