#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct virus virus;

struct virus {
    unsigned short length;
    char name[16];
    char signature[];
};

typedef struct link link;
 
struct link {
    virus *v;
    link *next;
};

int flag = 0;

void print_virus(virus* vrs) {
    if(vrs->length != 0) {
        printf("Virus name: %s\n", vrs->name);
        printf("Virus size %d\n", vrs->length);
        printf("signature:\n");
        int i;
        for(i = 0; i < vrs->length; i++) {
            printf("%02hhX ", vrs->signature[i]);
        }
        printf("\n\n");
    }
}

void list_print(link *virus_list) {
    struct link *tmp = virus_list;
    while(tmp != NULL) {
        print_virus(tmp->v);
        tmp = tmp->next;
    }
}

link* list_append(link* virus_list, virus* data) {
    if(data != NULL) {
        if(flag == 0){
            flag++;
            virus_list = malloc(sizeof(link));
            virus_list->v = data;
            virus_list->next = NULL;
            return virus_list;
        }
        else {
            link *last = malloc(sizeof(link));
            last->v = data;
            last->next = NULL;
            struct link *tmp = virus_list;
            while(tmp->next != NULL) {
                tmp = tmp->next;
            }
            tmp->next = last;
            return virus_list;
        }
    }
    return virus_list;

}

void list_free(link *virus_list) {
    struct link *nextLink = virus_list->next;
    while(nextLink != 0){
            free(virus_list->v);
            free(virus_list);
            virus_list = nextLink;
            nextLink = virus_list->next;
    }
    free(virus_list->v);
    free(virus_list);
}

int cmp_str(const void *str1, const void *str2, size_t n) {
    int i;
    unsigned char *a;
    unsigned char *b;
    for(i = 0; i < n; i++) {
        a = str1;
        b = str2;
        if(*a != *b)
            return 1;
        str1++;
        str2++;
    }
    
    return 0;
}


void detect_virus(char *buffer, link *virus_list, unsigned int size) {
    int i;
    while(virus_list != NULL){
        struct virus *vrs = virus_list->v;
        i = 0;
        while(i < size){
            while(buffer[i] != vrs->signature[0] && i < size) {
                i++;
            }
            if(memcmp(buffer + i, vrs->signature, vrs->length) == 0){
                printf("Virus starting byte: %d\n", i);
                printf("Virus name: %s\n", virus_list->v->name);
                printf("Virus size: %d\n", virus_list->v->length);
                printf("\n");

                break;
            }
            else
                i++;
        }
        if(virus_list->next != NULL) 
            virus_list = virus_list->next;
        else 
            break;
        
    }
}




int main(int argc, char **argv) {
    struct virus *vrs;
    struct link *virus_list;
    FILE *file = stdout;
    file = fopen( argv[1], "r" );
    char len[2];

    int i = 0, f = 0;
    i = fread (len,1,1,file);
    if(len[0] == 1)
        f = 1; 
    if(i == 1) {
        while(1) {
            i = fread (len,1,2,file);
            if(i == 2) {
                vrs = malloc(sizeof(virus) + len[1] * sizeof(vrs->signature[0]));
                if( f != 1)
                    vrs->length = len[0] - 18;
                else
                    vrs->length = len[1] - 18;
                i = fread (vrs->name, 1, 16, file);
                if(i  == 16) {
                    i = fread (vrs->signature,1,vrs->length,file);
                }
                virus_list = list_append(virus_list, vrs);
            }
            else {
                break;
            }
        }
    }
    list_print(virus_list);
    char buffer[10000];
    int z;
    for(z = 0; z< 10000;z++)
        buffer[z] = 0;
    size_t size;
    close(file);
    file = fopen(argv[2], "r" );
    size = fread(buffer,1, 10000 , file);
    close(file);

    detect_virus(buffer, virus_list, size);

    return 0;
}


























