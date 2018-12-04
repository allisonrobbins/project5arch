#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hashmap.h"

void training(struct hashmap *hm);
void read_query(struct hashmap *hm);
void training(struct hashmap* hm)
{
    printf("made it here");
    FILE *fptr;
    char line[256];
    char* word;
    fptr = fopen("D1.txt", "r");//open file 1
    printf("opened file 1");
    fgets(line,256,fptr);
    word = strtok(line, " ");
    while(1)//the loop to read file D1.txt into the hashmap
    {
        if(word == NULL)
        {
            break;
        }
        printf(" %s",word);
        hm_put(hm,word,1,0,0);
        word = strtok(NULL," ");
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
        word = strtok(NULL," ");
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
        word = strtok(NULL," ");
    }
    fclose(fptr);
}
void read_query(struct hashmap *hm)
{
    printf("Enter a string you want to search for, then press enter:\n");
    char input[256];
    scanf("\n");
    fgets(input, 256, stdin);
    printf("You entered %s\n",input);
    char* word;
    word = strtok(input," ");
    while(word!=NULL)
    {
        struct llnode* ugh = hm_get(hm,word);
        printf("%s",ugh->word);
        word = strtok(NULL, " ");
    }
    
}
int main(void)
{
    printf("Enter the number of buckets you want the hashmap to have: \n");
    int numBuckets;
    scanf("%i", &numBuckets);
    struct hashmap *hm = hm_create(numBuckets);
    printf("created hashmap of size %i\n",numBuckets);
    //training(hm);
    while(1)
    {
        printf("Enter S to search and X to exit: \n");
        char choice;
        scanf("%c", &choice);
        if(choice == 'S')
        {
            read_query(hm);
        }
        else if(choice == 'X')
        {
            break;
        }
    }
    training(hm);
    
    /*
    printf("___________________________________________________________\n");
    printf("Create the hashmap and input files:\n");
    struct hashmap *hm = hm_create(23);
    printf("I made a hashmap\n");
    training(hm);
    printf("___________________________________________________________\n");*/
}
