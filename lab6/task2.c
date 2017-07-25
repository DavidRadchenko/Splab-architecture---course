#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <linux/limits.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/syscall.h>
#include <linux/limits.h>
#include <sys/types.h>



int dflag = 0, ret = 0;

int main(int argc, char **argv) {
    int fds[2];
    char* ls[3] = {"ls", "-l", 0};
    char* tail[4] = {"tail", "-n", "2", 0};

    int pid1 = 0, pid2 = 0;
    int status = 0;
    int i = 0;
    for(i = 1; i < argc; i++) 
        if(argv[i][0] == '-' && argv[i][1] == 'd') 
            dflag = 1;

    ret = pipe(fds);
    if(ret == -1) {
        perror("pipe");
        exit(1);
    }

    if(dflag)
        fprintf(stderr, "parent_process>forking...");
    pid1 = fork();
   

    if(pid1 == 0){
        if(dflag)
            fprintf(stderr, "Child1 process> redirecting end of pipe");
    	close(1);
    	dup(fds[1]);
    	close(fds[1]);
        if(dflag) 
            fprintf(stderr, "Child1 process> going to execute cmd");
    	execvp(ls[0], ls);
    }
    else  {
         if(dflag) {
            fprintf(stderr, "Parent process> created process with id: %d\n", pid1);
            fprintf(stderr, "parent_process>closing the write end of the pipe…");
         }
        close(fds[1]); 
    	pid2 = fork();
	    if(pid2 == 0){
	    	close(0);
	    	dup(fds[0]);
	    	close(fds[0]);
	    	execvp(tail[0], tail);
	    }
	    else {
	    	if(dflag)
                fprintf(stderr, "Parent process> closing the read end of pipe\n");
            close(fds[0]);

            if(dflag)
                fprintf(stderr, "Parent process> waiting for children to end\n");
            waitpid (pid1, &status, 0);
            waitpid (pid2, &status, 0);

            if(dflag)
                fprintf(stderr, "STDERR: Parent process> exiting\n");
	    }
	 } 
    return 0;
}
