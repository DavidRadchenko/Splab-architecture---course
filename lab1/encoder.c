#include <stdio.h>
#include <string.h>

int encode(int x, char sign, char num);
int main ( int argc, char *argv[] ) {
    char sign = '-';
    char num = 'a';
    FILE *file = stdout;
    int i, x;
    for(i=1; i<argc; i++) {
            if(argv[i][0] == '+' || (argv[i][0] == '-' && (strcmp(argv[i],"-i")!=0 && strcmp(argv[i],"-o")!=0))) {
                if (argv[i][0] == '+')
                    sign = '+';
                num = argv[i][1];
            }
    }
    for(i=1; i<argc; i++) {
            if(strcmp(argv[i],"-i")==0) {
                file = fopen( argv[++i], "r" );
                if ( file == 0 ) {
                    printf( "Could not open file\n" );
                    break;
                }
                else {
                    while  ( (x = fgetc( file )) != EOF ) {
                        printf( "%c", encode(x, sign, num) );
                    }
                }
            }
            else if(strcmp(argv[i],"-o")==0) {
                char output[30];
                printf( "Enter output file:\n" );
                int k = 0;
                while  ( (x = fgetc( stdin) ) != '\n' && k < 30) {
                    output[k] = x;
                    k = k + 1;
                }
                char str[k];
                /*printf("%c\n", k);

                printf("%s\n", output);*/
                int z;
                for(z=0; z<k; z++) {
                    str[z] = output[z];
                }
                /*printf("%s\n", str);*/

                file = fopen( output, "w" );
                if ( file == 0 ) {
                    printf( "Could not open file\n" );
                    break; 
                }
                else {
                    printf( "Enter text to encode:\n" );
                    while  ( (x = fgetc( stdin )) != EOF ) {
                        fputc( encode(x, sign, num), file );
                    }
                    fclose(file);
                    file = fopen( output, "r" );
                    char line[150];

                    while (1) {
                        if (fgets(line,150, file) == NULL) break;
                        printf("%s", line);
                    }
                }
            }
        }
    if(argc <= 2) {
        while( (x = fgetc( stdin )) != EOF ) {
            printf( "%c", encode(x, sign, num) );
        }
    }
    fclose(file);
    return 0;
}
 
int encode(int x, char sign, char num) {
    if ((x >= 65) && (x <= 90)) 
        x = x + 32; 
    if (( x >=97 ) && (x <=122)) {
        if(sign == '+' && num != 'a') {
        x = x + (num - 48);
        if (x - 122 > 0) 
            x = x - 26;
        }
        else if(sign == '-'&& num != 'a') {
            x = x - (num - 48);
            if (x - 97 < 0) 
                x = x + 26;
        }
    }
    return x;
}








