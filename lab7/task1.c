#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int dflag = 0, size = 0;
FILE *file;
char filename[100];
unsigned char* data_pointer;
char *password = "lab8";

struct fun_desc {
	char *name;
	void (*fun)();
};

void debugMode() { 
    char tmp[100];
	printf("Please enter password: ");
    fgets(tmp, 100, stdin);
    tmp[4] = 0;
	if(strcmp(tmp, password) == 0 && dflag == 0){
		dflag = 1;
		printf("Debug flag now on\n");
        return;
	}
	if(strcmp(tmp, password) == 0 && dflag == 1){
		dflag = 0;
		printf("Debug flag now off\n");
        return;
	}
	printf("Wrong Password!\n");
}

void setFile() {
    int i;
    for (i = 0; i < 100; i++) 
        filename[i] = 0;
    char tmp[100];
    printf("Enter file name: ");
    fgets(tmp, 100, stdin);
    strncpy (filename, tmp, strlen(tmp) - 1);
}

void setUnitSize() {
	printf("Enter unit size: ");
    size = fgetc(stdin);
    size = size - '0';
}

void fileDisplay() {
    if (filename == NULL) {        
        fprintf(stderr, "Filename wasn't initialized yet\n");
        return;
    }
    file = fopen(filename, "r");
    if (!file) {
        printf("Could not open file\n");
        return;
    }
    int location = 0, length = 0;
    char input[100];
    printf("Please enter <location> <length>\n");
    fgets(input, 100, stdin);
    sscanf(input,"%x %d",&location, &length);
    if (fseek (file , location , SEEK_SET) != 0) {
        printf("Could not find location in file\n");
        return;
    }   
    if (dflag) {
        printf("Debug: location set to %02x\n", location);
        printf("Debug: length set to %d\n", length); 
    }
    unsigned char *buff = malloc(size*length);
    fread(buff, size, length, file);
    fclose(file);
    int i, j;
    printf("Hexadecimal Representation:\n");
    for (i = 0; i < length; i++) {
        for(j = size - 1; j > -1; j--)
            printf("%02x", buff[i*size + j]);
        printf(" ");
    }
    printf("\n");
    int k = 0;
    printf("Decimal Representation:\n");
    for (i = 0; i < length; i++) {
        k = buff[i*size + size - 1]*256 + buff[i*size + size - 2];
        printf("%d ", k);
    }
    printf("\n");
    free(buff);
} 

void LoadToMry() {
    if (filename == NULL) {        
        fprintf(stderr, "Filename wasn't initialized yet\n");
        return;
    }
    file = fopen(filename, "r");
    if (!file) {
        printf("Could not open file\n");
        return;
    }
    int location = 0, length = 0, memAddr = 0;
    char input[100];
    printf("Please enter <mem-address> <location> <length>\n");
    fgets(input, 100, stdin);
    sscanf(input,"%d %x %d", &memAddr, &location, &length);
    if (data_pointer != NULL)
        free(data_pointer);
    data_pointer = malloc(length);
    if (fseek (file , location , SEEK_SET) != 0) {
        printf("Could not find location in file\n");
        return;
    }
    fread(data_pointer, 1, length, file);

    fclose(file);
    if (dflag) {
        printf("Debug: location set to %02x\n", location);
        printf("Debug: length set to %d\n", length); 
        printf("Debug: mem-address set to %d\n", memAddr); 
    }
    printf("Loaded %d bytes into %p\n", length, data_pointer);
} 

void saveToFile() {
    if (filename == NULL) {        
        fprintf(stderr, "Filename wasn't initialized yet\n");
        return;
    }
    file = fopen(filename, "r+");
    if (!file) {
        printf("Could not open file\n");
        return;
    }
    int location = 0, length = 0, sourceAddr = 0;
    char input[100];
    printf("Please enter <source-address> <target-location> <length>\n");
    fgets(input, 100, stdin);
    sscanf(input,"%x %x %d", &sourceAddr, &location, &length);
    if (dflag) {
        printf("Debug: target-location set to %02x\n", location);
        printf("Debug: length set to %d\n", length); 
        printf("Debug: source-address set to %d\n", sourceAddr); 
    }
    if (sourceAddr == 0)
        sourceAddr = data_pointer;
    char* buff = sourceAddr;
    if (fseek(file , location , SEEK_SET) != 0) {
        printf("Could not find location in file\n");
        return;
    }
    int j = 0;
    j = fwrite(buff, 1, length, file);
    if (j != length)
        printf("Shit\n");
    fclose(file);
}

void fileModify() {
    if (filename == NULL) {        
        fprintf(stderr, "Filename wasn't initialized yet\n");
        return;
    }
    file = fopen(filename, "r+");
    if (!file) {
        printf("Could not open file\n");
        return;
    }
    if (size == 0) {        
        fprintf(stderr, "UNIT-SIZE wasn't initialized yet\n");
        return;
    }
    char input[100];
    int location = 0, val = 0;
    printf("Please enter <location> <val>\n");

    fgets(input, 100, stdin);
    sscanf(input,"%x %x", &location, &val);

    if (fseek(file , location , SEEK_SET) != 0) {
        printf("Could not find location in file\n");
        return;
    }

    int j = 0;
    j = fwrite(&val, 1, size, file);
    if (j != size)
        printf("Shit\n");


    if (dflag) {
        printf("Debug: target-location set to %02x\n", location);
        printf("Debug: val set to %02x\n", val); 
    }
    fclose(file);  
}

void quit() {
    free(data_pointer);
    exit(0);
} 

void map(void (*f) ()) {
	f();   
}

struct fun_desc menu[] = { { "0-Toggle Debug Mode", &debugMode }, { "1-Set File Name", &setFile }, 
{ "2-Set Unit Size", &setUnitSize }, { "3-File Display", &fileDisplay }, { "4-Load Into Memory", &LoadToMry }, 
{ "5-Save Into File", &saveToFile }, { "6-File Modify", &fileModify }, { "7-Quit", &quit }, { NULL, NULL } };

int main(int argc, char **argv){
	struct fun_desc* ptr = menu;
    int menuSize = 7;
    while (1) {
        int z;
        if (dflag) {
            printf("Debug: size set to %d\n", size);
            printf("Debug: file name set to %s\n", filename); 
        }
        printf("Please choose a function:\n"); 
        while (ptr->name != NULL) {
            printf("%s\n", ptr->name); 
            ptr++; 
        }
        printf("Option: "); 
        scanf("%d",&z);
        fgetc(stdin);
        if (z >= 0 && z <= menuSize) {
            map(menu[z].fun);
            printf("\n"); 
            ptr = menu;
        }
        else {
            printf("Not within bounds\n"); 
            free(ptr);
            quit('c');
        }   
    }
    quit('c');
    return 0;
}



















































