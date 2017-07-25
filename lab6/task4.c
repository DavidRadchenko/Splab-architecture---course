#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <linux/limits.h>
#include  <sys/types.h>
#include "LineParser.h"
#include <sys/wait.h>

typedef struct shellVar
{
    char* name;
    char* value;
    struct shellVar *next;
} shellVar;

int dflag = 0;
int fds[2];
FILE *fp1, *fp2;

shellVar* var_list;


void setShellVar(char* name, char* value) {
    
    shellVar* newVar = malloc(sizeof(shellVar));
    newVar -> name = malloc(strlen(name)+1);
    strncpy(newVar -> name, name, strlen(name) + 1);
    newVar->value = malloc(strlen(value)+1);
    strncpy(newVar -> value, value, (strlen(value)+1));
    newVar -> next = NULL;
    if (var_list == NULL) {
        var_list = newVar;
    }
    else {
        shellVar* list = var_list;
        while (list -> next !=NULL)
            list = list -> next;
        list -> next = newVar;
	}
}

void printShellVars() {
    shellVar* tmp_list = var_list;

    while(tmp_list != NULL){
            printf("Name: %s Value: %s\n",tmp_list -> name, tmp_list -> value);
            tmp_list = tmp_list -> next;
    }
 }

char* returnShellVar(char* name) {
    shellVar* tmp_list = var_list;

    while(tmp_list != NULL){

        if(tmp_list -> name[0] == name[0]){
            return tmp_list -> value;
 
        }
        tmp_list = tmp_list -> next;
    }
    fprintf(stderr,"No such variable\n"); 
    return NULL;
}

void freeVarList() {
    if (var_list == NULL)
        return;
    while (var_list != NULL){
        shellVar* tmp = var_list;
        var_list = var_list -> next;
        free(tmp -> name);
        free(tmp -> value);
        free(tmp);
    }
}

void deleteShellVars(char* name) {
    if (var_list == NULL) {
        printf("Bullshit\n"); 
        fprintf(stderr,"No such variable\n"); 
        return;
    }
	shellVar* tmp_list = var_list;
	if (strcmp(tmp_list->name, name) == 0){
        shellVar* tmp = var_list;
		var_list = tmp_list -> next;
		free(tmp -> name);
        free(tmp -> value);
        free(tmp);
		return;
	}
    while (tmp_list-> next != NULL) {
        if(strcmp((tmp_list -> next) -> name, name) == 0) {
            shellVar* tmp = tmp_list-> next;
            tmp_list -> next = tmp -> next;
            free(tmp -> name);
            free(tmp -> value);
            free(tmp);
            return;
        }
        tmp_list = tmp_list -> next;
	}
    fprintf(stderr,"No such variable\n"); 
}

void execute(cmdLine *pCmdLine) {
	int cPid = 0, fPid = 0, *status = 0, a = 0;
	cPid = fork();
	if (cPid == 0) {
		a = execvp(pCmdLine -> arguments[0], pCmdLine->arguments);
		if (a == -1) {
		  	fprintf(stderr,"exec failed\n"); 
                        freeCmdLines(pCmdLine);
		  	exit(0);
		}
	}
        else {
            a = pCmdLine -> blocking;
            if(a) {
                fPid = (pid_t)waitpid(cPid, status, 0);
                if (fPid == -1)
                fprintf(stderr, "error");
            }
    }
    if (dflag)
        fprintf(stderr,"PID: %d, Executing command: %s", cPid, pCmdLine -> arguments[0]);
    
}

int main(int argc, char **argv) {

        char buff[PATH_MAX + 1];
        char input[2048];
        char *val;
        cmdLine *cmdl;
        int i = 0;
        for(i = 1; i < argc; i++) 
            if(argv[i][0] == '-' && argv[i][1] == 'd') 
                dflag = 1;
        i = 0; 
        while(1) {
            
            getcwd( buff, PATH_MAX + 1 );
                if(buff != NULL) 
                    printf("DIR:%s\n", buff);
            fgets(input, 2048, stdin);

            int k = 0;
            cmdl = parseCmdLines (input);
            while(k < cmdl->argCount) {
                if(cmdl->arguments[k][0] == '$') {
                    val = returnShellVar(&cmdl->arguments[k][1]);
                    replaceCmdArg(cmdl, k, val);
                }
                k++;
            }
            
            if (strcmp(cmdl->arguments[0], "quit") == 0) {
                freeCmdLines(cmdl);
                freeVarList();
                exit(0);
            }
            else if (strcmp(cmdl->arguments[0], "cd") == 0) {
            	int ret;
            	if(cmdl->arguments[1][0] == '~')
            		ret = chdir(getenv("HOME"));
            	else
                	ret = chdir(cmdl->arguments[1]);
                if (ret < 0)
                    fprintf(stderr, "cd failed");
            } 
            else if (strcmp(cmdl->arguments[0], "set") == 0) {
                if(i < 100)
                    setShellVar(cmdl->arguments[1], cmdl->arguments[2]);
                i++;
            }
            else if (strcmp(cmdl->arguments[0], "env") == 0) {
                
                    printShellVars();
            }
            else if (strcmp(cmdl->arguments[0], "delete") == 0) {
                    deleteShellVars(cmdl->arguments[1]);
            } 
            else 
                execute (cmdl);     
            freeCmdLines(cmdl);
        }
        return 0;
}
