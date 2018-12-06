#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "hashmap.h"

void training(struct hashmap *hm);
void read_query(struct hashmap *hm);
void stop_words(struct hashmap *hm);
double findInvDocFreq(struct llnode* a);
double tf_idf(struct llnode* a, int docNum);
int findDocFreq(struct llnode* a);
double rank(struct llnode* a, int docNum);
void sort(double R1, double R2, double R3);
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
void stop_words(struct hashmap *hm)
{
  int i;
  for(i=0; i<hm->num_buckets; i++)
  {
    struct llnode* iter = hm->map[i];
    if(strcmp(iter->word,"null")==0)
    {
      break;
    }
    else if(iter->D1count>=1 && iter->D2count>=1 && iter->D3count>=1)
    {
      hm_remove(hm,iter->word);
    }
  }
}
void read_query(struct hashmap *hm)
{
    printf("Enter a string you want to search for, then press enter:\n");
    char input[256];
    scanf("\n");
    fgets(input, 256, stdin);
    double R1 = 0;
    double R2 = 0;
    double R3 = 0;
    printf("You entered %s\n",input);
    char* word;
    word = strtok(input," ");

    while(word!=NULL)
    {
        struct llnode* ugh = hm_get(hm,word);
      	if(strcmp(ugh->word,"null")!=0)
      	{
            R1 += rank(ugh,1);
            R2 += rank(ugh,2);
            R3 += rank(ugh,3);
        }
      	word = strtok(NULL, " ");
    }
    if(R1+R2+R3 == 0)
    {
      printf("Error: none of those words are in the document\n");
    }
    else
    {
      sort(R1,R2,R3);
    }
}
void sort(double R1, double R2, double R3)
{
  if(R1>=R2 && R2>=R3)
  {
    printf("D1.txt, D2.txt, D3.txt\n");
  }
  else if(R1>=R3 && R3>=R2)
  {
    printf("D1.txt, D3.txt, D2.txt\n");
  }
  else if(R2>=R1 && R1>=R3)
  {
    printf("D2.txt, D1.txt, D3.txt\n");
  }
  else if(R2>=R3 && R3>=R1)
  {
    printf("D2.txt, D3.txt, D1.txt\n");
  }
  else if(R3>=R2 && R2>=R1)
  {
    printf("D3.txt, D2.txt, D1.txt\n");
  }
  else
  {
    printf("D3.txt, D1.txt, D2.txt\n");
  }
}
double rank(struct llnode* a, int docNum)
{
  double b = findInvDocFreq(a);
  if(docNum==1)
  {
    return (a->D1count)*b;
  }
  else if(docNum == 2)
  {
    return (a->D2count)*b;
  }
  else
  {
    return (a->D3count)*b;
  }
}
int findDocFreq(struct llnode* a)
{
    int numDoc=0;
    if(a->D1count>0)
    {
      numDoc++;
    }
    if(a->D2count>0)
    {
      numDoc++;
    }
    if(a->D3count>0)
    {
      numDoc++;
    }
    return numDoc;
}
double findInvDocFreq(struct llnode* a)
{
    int numDoc=0;
    if(a->D1count>0)
    {
      numDoc++;
    }
    if(a->D2count>0)
    {
      numDoc++;
    }
    if(a->D3count>0)
    {
      numDoc++;
    }
    return log10(3/numDoc);
}
int main(void)
{
  printf("Enter the number of buckets you want the hashmap to have: \n");
  int numBuckets;
  scanf("%i", &numBuckets);
  struct hashmap *hm = hm_create(numBuckets);
  training(hm);
  stop_words(hm);
  while(1)
  {
      printf("Type S to search or X to exit, then press enter: \n");
      char choice;
      choice = getchar();
      printf("You entered: ");
      putchar(choice);
      printf("\n");
      if(choice == 'S'||choice == 's')
      {
          read_query(hm);
      }
      else if(choice == 'X'||choice == 'x')
      {
          break;
      }
      else
      {
          printf("Error: that char was invalid. \n");
      }
  }
  hm_destroy(hm);
}
