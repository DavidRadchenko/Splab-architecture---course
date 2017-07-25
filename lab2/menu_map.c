#include <stdio.h>
#include <string.h>
#include <stdlib.h>



char censor(char c) {
  if(c == '!')
    return '.';
  else
    return c;
}


/* Gets a char c and returns its encrypted form by adding 3 to its value. 
   If c is not between 0x20 and 0x7E it is returned unchanged */
char encrypt(char c) {
	if( c >= 0x20 && c <= 0x7E) 
    return (c + 3);
	return c;
}

/* Gets a char c and returns its decrypted form by reducing 3 to its value. 
   If c is not between 0x20 and 0x7E it is returned unchanged */
char decrypt(char c) {
	if( c >= 0x20 && c <= 0x7E) 
		return (c - 3);
	return c;
}

/* xprt prints the value of c in a hexadecimal representation followed by a 
   new line, and returns c unchanged. */
char xprt(char c) {
	printf("%#04x\n", c); 
	return c;
}

/* If c is a number between 0x20 and 0x7E, cprt prints the character of ASCII value c followed by a new line. 
   Otherwise, cprt prints the dot ('.') character. After printing, cprt returns the value of c unchanged. */
char cprt(char c) {
	if(c >= 0x20 && c <= 0x7E) 
    printf("%c\n", c); 
	else 
    printf(".\n");
	return c;
}

/* Ignores c, reads and returns a character from stdin using fgetc. */
char my_get(char c) {
	char x;
     scanf("%c",&x);
	return x;
}

/* Gets a char c, and ends the program using c as the return value */
char quit(char c) {
	exit(c);
	return c;
} 


 struct fun_desc {
  char *name;
  char (*fun)(char);
};

struct fun_desc menu[] = { { "Censor", &censor }, { "Encrypt", &encrypt }, { "Decrypt", &decrypt }, 
{ "Print hex", &xprt }, { "Print string", &cprt }, { "Get string", &my_get }, { "Quit", &quit }, { NULL, NULL } };

char* map(char *array, int array_length, char (*f) (char)){
    char* mapped_array = (char*)(malloc(array_length*sizeof(char)));
    int i = 0;
    char z;
    while(i < array_length) {
      mapped_array[i] = f(array[i]);
      i++;
    }
    return mapped_array;
}

int main(int argc, char **argv){

  char carray[5];
	struct fun_desc* ptr = menu;
	struct fun_desc* p = menu;
  char* arr2;

	int i = 0;

	while (p->name != NULL) {
	  p++; 
	  i++;
	}
        while(1) {
            int z;
            printf("Please choose a function:\n"); 
            while (ptr->name != NULL) {
                printf("%s\n", ptr->name); 
                ptr++; 
            }
            printf("Option: "); 
            scanf("%d",&z);

            if(z >= 0 && z <= i) {
                    printf("Within bounds\n"); 
                    arr2 = map(carray, 5, menu[z].fun);
                    printf("Done.\n"); 
                    strcpy(carray, arr2);
                    ptr = menu;
            }
            else {
                    printf("Not within bounds\n"); 
                    free(ptr);
                    free(p);
                    free(arr2);
                    quit('c');
            }
        }
        quit('c');
        return 0;
}



















































