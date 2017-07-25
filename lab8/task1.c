#include <stdio.h>
#include <stdlib.h>
#include "elf.h"
#include <sys/stat.h>
#include <sys/mman.h>

#define O_RDONLY 0


int dflag = 0, size = 0;
char filename[100];
unsigned char* data_pointer;
int Currentfd = -1;

int fd;
void *map_start; /* will point to the start of the memory mapped file */
struct stat fd_stat; /* this is needed to the size of the file */
Elf32_Ehdr *header; /* this will point to the header structure */
int num_of_section_headers;


struct fun_desc {
	char *name;
	void (*fun)();
};

void debugMode() {
	if (!dflag) {
		dflag = 1;
		printf("Debug flag now on");
        return;
	}
  	dflag = 0;
    printf("Debug flag now off");
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

void examineElfFile() {
    setFile();

    if(Currentfd != -1) { 
        close(Currentfd);
    }

    Currentfd = open(filename, O_RDONLY);
    if (!Currentfd) {
        printf("Could not open file\n");
        return;
    }

    if( fstat(Currentfd, &fd_stat) != 0 ) {
        fprintf(stderr, "stat failed");
        return;
   }

   map_start = mmap(0, fd_stat.st_size, PROT_READ, MAP_PRIVATE, Currentfd, 0);
   if (map_start == MAP_FAILED) {
      fprintf(stderr, "Error in mmap");
      return;
   }

   header = (Elf32_Ehdr *) map_start;
   if(header->e_ident[1] != 'E' || header->e_ident[2] != 'L' || header->e_ident[3] != 'F') {
        fprintf(stderr, "Not ELF File");
        munmap(map_start, fd_stat.st_size);
        close(Currentfd);
        Currentfd = -1;
        return;
    }
    printf("Byte 1, 2 and 3: %c, %c, %c\n", header->e_ident[1], header->e_ident[2], header->e_ident[3]);
    printf("File encoding: 0x%x\n", header->e_ident[5]);
    printf("Entry point address: %02x\n", header->e_entry);
    printf("The file offset in which the section header table resides: %x\n", header->e_shoff);
    printf("Number of section headers %d\n", header->e_shnum);
    printf("Size of section headers (bytes): %d \n", header->e_shentsize);
    printf("Start of program headers (bytes into file): %d \n", header->e_phoff);
    printf("Number of program headers: %d\n", header->e_phnum);
    printf("Size of program headers (bytes): %d\n", header->e_phentsize );
}


void printSectionNames() {
    if (Currentfd == -1) {
        printf("ERROR WITH Currentfd\n");
        return;
    }
    Elf32_Shdr* sections_header = (Elf32_Shdr *)(map_start + header->e_shoff);
    Elf32_Shdr* section_string_table = &sections_header[header->e_shstrndx];
    int sectionsNum = header->e_shnum;
    char* const section_string_table_p = map_start + section_string_table->sh_offset;
    if(dflag) 
        printf("shstrndx: 0x%x\n", header->e_shstrndx);


    printf("[index]:section_name | section_address | section_offset | section_size | section_type\n");
    int i;
    for(i = 1; i < sectionsNum; i++) {

        printf("[%2d]:%20s | %10x | %10x | %10x | %10x", i,
        section_string_table_p + sections_header[i].sh_name,
        sections_header[i].sh_addr,
        sections_header[i].sh_offset,
        sections_header[i].sh_size,
        sections_header[i].sh_type);
        printf("\n");
    }
}

void PrintSymbols() {
    if (Currentfd == -1) {
        printf("ERROR WITH Currentfd\n");
        return;
    }
    Elf32_Shdr* sections_header = (Elf32_Shdr *)(map_start + header->e_shoff);
    Elf32_Shdr* section_string_table = &sections_header[header->e_shstrndx];
    Elf32_Sym* section_symbol_table;
    int sectionsNum = header->e_shnum;
    char* symbol_string_table;
    char* const section_string_table_p = map_start + section_string_table->sh_offset;

    printf("[index]:section_name | section_address | section_offset | section_size | section_type\n");
    int i;

    for(i = 0; i < sectionsNum; i++) {
    }
}


void quit() {
    close(Currentfd);
    munmap(map_start, fd_stat.st_size);
    exit(0);
} 

void map(void (*f) ()) {
	  f();   
}

struct fun_desc menu[] = {{ "0-Toggle Debug Mode", &debugMode },  
                          { "1-Examine ELF File", &examineElfFile }, 
                          { "2-Print Section Names", &printSectionNames }, 
                          { "3-Print Symbols", &PrintSymbols }, 
                          { "4-Quit", &quit }, 
                          { NULL, NULL }};

int main(int argc, char **argv){
	  struct fun_desc* ptr = menu;
    int menuSize = 7;
    while (1) {
        int z;
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



















































