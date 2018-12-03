#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hashmap.h"


void readFiles(struct hashmap *hm);
void readFiles(struct hashmap* hm)
{
    FILE *fptr;
    char line[256];
    char* word;
    fptr = fopen("D1.txt", "r");//open file 1
    if(fptr == NULL)
    {
        return;
    }
    fgets(line,256,fptr);
    word = strtok(line, " ");
    while(1)//the loop to read file D1.txt into the hashmap
    {
        if(word == NULL)
        {
            break;
        }
        hm_put(hm,word,1,0,0);
        char* word = strtok(NULL," ");
    }
    fclose(fptr);//close file 1
    fptr = fopen("D2.txt", "r");//open file 2
    if(fptr == NULL)
    {
        return;
    }
    fgets(line,256,fptr);
    word = strtok(line, " ");
    while(1)//the loop to read file D2.txt into the hashmap
    {
        if(word == NULL)
        {
            break;
        }
        hm_put(hm,word,0,1,0);
        char* word = strtok(NULL," ");
    }
    fclose(fptr);//close file 2
    fptr = fopen("D3.txt", "r");//open file 3
    if(fptr == NULL)
    {
        return;
    }
    fgets(line,256,fptr);
    word = strtok(line, " ");
    while(1)//the loop to read D3 into the hashmap
    {
        if(word == NULL)
        {
            break;
        }
        hm_put(hm,word,0,0,1);
        char* word = strtok(NULL," ");
    }
    fclose(fptr);
}

int main(void)
{
    printf("___________________________________________________________\n");
    printf("Create the hashmap and input files:\n");
    struct hashmap *hm = hm_create(23);
    printf("I made a hashmap\n");
    readFiles(hm);
    printf("___________________________________________________________\n");
}

