#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hashmap.h"


void readFile(char* input_file, struct hashmap *hm);
void hm_print(struct hashmap* hm);
void readFile(char* input_file,struct hashmap *hm)
{
  printf("reading file %s\n",input_file);
  FILE *fptr;
  char line[256];
  char* word;
  fptr = fopen(input_file, "r");
  if(fptr == NULL)
  {
    return;
  }
  fgets(line,256,fptr);
  word = strtok(line, " ");
  while(word!=NULL)
  {
    hm_put(hm,word,input_file,1);
    word = strtok(NULL, " ");
  }
  fclose(fptr);
}

void hm_print(struct hashmap* hm)
{
  int i;
  struct llnode* iter;
  printf("printing hashmap\n");
  printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  for(i=0; i<hm->num_buckets; i++)
  {
    iter = hm->map[i];
    printf("Bucket number %i:\n",i);
    printf("\n");
    if(iter==NULL)
    {
      printf("There is nothing in the bucket\n");
      printf("\n");
    }
    while(iter!=NULL)
    {
      printf("Word = %s, Document ID = %s, Number of Occurrences = %i\n",iter->word,iter->document_id,iter->num_occurrences);
      iter = iter->next;
    }
    printf("\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  }
}
int main(void)
{
  printf("___________________________________________________________\n");
  printf("Create the hashmap and input files:\n");
  struct hashmap *hm = hm_create(23);
  printf("I made a hashmap\n");
  readFile("input1.txt",hm);
  readFile("input2.txt",hm);
  readFile("input3.txt",hm);
  hm_print(hm);
  printf("___________________________________________________________\n");
  printf("Test get ~you~ from input2.txt (should be 6): %d\n", hm_get(hm, "you", "input2"));
	hm_remove(hm, "you", "input2");
	printf("After remove: test get \"homework\" from D3.txt (should be -1): %d\n", hm_get(hm, "you", "input2"));
	printf("After remove: \"homework\" -> num_elements: %d\n", hm->num_elements);
printf("___________________________________________________________\n");
	hm_destroy(hm);
  printf("\n");
  printf("___________________________________________________________\n");

}

