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
  for(i=0; i<hm->num_buckets; i++)//go through each bucket
  {
    struct llnode* iter = hm->map[i];
    if(strcmp(iter->word,"null")==0)//tet if the first node is null if it is then go to next bucket
    {
      break;
    }
    else if(iter->D1count>0 && iter->D2count>0 && iter->D3count>0)
    {//see if the word is in all documents, aka the counter for that document is positive
      hm_remove(hm,iter->word);//if a stop word, then delete that node
    }
  }
}
void read_query(struct hashmap *hm)
{//this method reads in a string from the user and computes the result
    printf("Enter a string you want to search for, then press enter:\n");
    char input[256];
    scanf("\n");//search query ends when the user presses enter
    fgets(input, 256, stdin);
    double R1 = 0;//these are the tallies of the tf_idf for each document
    double R2 = 0;
    double R3 = 0;
    char* word;
    word = strtok(input," ");//reading one word at a time

    while(word!=NULL)//keep going until the end of the search query
    {
        struct llnode* input = hm_get(hm,word);//get the corresponding llnode to the word
      	if(strcmp(input->word,word)==0)//double check that it is the right node
      	{
            R1 += rank(input,1);//increment the tf_idf count for this word
            R2 += rank(input,2);
            R3 += rank(input,3);
        }
      	word = strtok(NULL, " ");//move to next word
    }//after all words have been added to the tf_idf
    if(R1+R2+R3 == 0)//if the total sum is 0, then there are no matches in the hashmap
    {
      printf("Error: none of those words are in the document\n");
    }
    else//now that all the tf_idf counts are up to date, sort and print to console
    {
      sort(R1,R2,R3);//call sort method
    }
}
void sort(double R1, double R2, double R3)
{//This method sorts the documents in order of relevance and then prints them to the console in that order
  //instead of using a sorting algorithm, because it is only 3 documents, I used cascading if/else statements
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
  double b = findInvDocFreq(a);//the inverse document frequency is the same no matter which specific document we are looking for
  if(docNum==1)//now, multiply idf by the corresponding count for the specific document and return that value
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
double findInvDocFreq(struct llnode* a)
{
    int numDoc=0;//counter for which documents the word appears
    if(a->D1count>0)//test each document counter for the node
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
    return log10(3/numDoc);//using math.h to compute the log of 3/document frequency
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
  {//for some reason, it doesn't wait for user input the first time, but then it works right the rest of the time
      printf("Type S to search or X to exit, then press enter: \n");
      char choice;
      choice = getchar();
      printf("You entered: ");
      putchar(choice);
      printf("\n");
      if(choice == 'S'||choice == 's')//case insensitive!!!
      {
          read_query(hm);//go to corresponding method
      }
      else if(choice == 'X'||choice == 'x')//wow also case insensitive
      {
          break;//break out of while loop to end program
      }
      else
      {
          printf("Error: that char was invalid. \n");//if not one of those chars, prompts for input again
      }
  }
  hm_destroy(hm);//when while loop is over, destroy the hashmap
}
