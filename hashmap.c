#include "hashmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct hashmap* hm_create(int num_buckets)
{
    struct hashmap* hm = (struct hashmap*)calloc(1,sizeof(struct hashmap));
    hm->map = (struct llnode**)calloc(num_buckets,sizeof(struct llnode));
    hm->num_buckets = num_buckets;
    hm->num_elements = 0;
    int i;
    for(i=0; i<num_buckets; i++)
    {
        hm_initBucket(hm,i);//calling this method makes all the buckets empty, then they can point to nodes
    }
    return hm;
}
void hm_initBucket(struct hashmap* hm, int index)
{
    struct llnode* temp = hm->map[index];
    temp->word = NULL;
    temp->D1count = 0;
    temp->D2count = 0;
    temp->D3count = 0;
    temp->next = NULL;
}
struct llnode* hm_get(struct hashmap* hm, char* word)
{
    int bucket = hash(hm,word);
    struct llnode* head = hm->map[bucket];
    struct llnode* iter = head;
    if (iter->next = NULL)
    {
        return iter;//return a null node
    }
    while(iter->next != NULL)
    {
        if(iter->word == word)
        {
            return iter;
        }
    }
    if(iter->word == word)
    {
        return iter;
    }
    else
    {
        return head;//head is null, so if the word is not found, return a null node
    }
}
/* This method takes a given word-document pair and updates the num_occurrences if it exists
*  or it creates a new llnode for that pair and inputs the num_occurrences there
*/
void hm_put(struct hashmap* hm, char* word, int D1, int D2, int D3)
{
    int bucket = hash(hm,word);
    struct llnode* head = hm->map[bucket];
    struct llnode* iter = head;
    if(iter->next = NULL)//first node in the bucket
    {
        struct llnode* newNode = (struct llnode*)calloc(1,sizeof(struct llnode));
        newNode->word = word;
        newNode->D1count += D1;
        newNode->D2count += D2;
        newNode->D3count += D3;
        newNode->next = NULL;
        iter->next = newNode;
        hm->num_elements++;
        return;
    }
    else
    {
        iter = iter->next;
        while(iter->next != NULL)
        {
            if (strcmp(iter->word, word) == 0)//word already has a bucket in the hashmap
            {
                iter->D1count +=D1;
                iter->D2count += D2;
                iter->D3count += D3;
                return;
            }
            iter = iter->next;
        }
        if (strcmp(iter->word, word) == 0)//word already has a bucket in the hashmap
        {
            iter->D1count +=D1;
            iter->D2count += D2;
            iter->D3count += D3;
            return;
        }
        else
        {
            struct llnode* newNode = (struct llnode*)calloc(1,sizeof(struct llnode));
            newNode->word = word;
            newNode->D1count += D1;
            newNode->D2count += D2;
            newNode->D3count += D3;
            newNode->next = NULL;
            iter->next = newNode;
            hm->num_elements++;
            return;
        }
    }
}
int hash(struct hashmap* hm, char* word, char* document_id)
{
    char* a;
    int i;
    int sum = 0;
    char* getRidOfDumbError = document_id;
    for(a = word; *a!='\0'; a++)
    {
      i = (int) *a;
      sum = sum + i;
    }
    sum = sum%hm->num_buckets;
    getRidOfDumbError++;
    return sum;
}
void hm_destroy(struct hashmap* hm)
{
    int i;
    for(i=0; i<hm->num_buckets; i++)
    {
      printf("destroying bucket %i\n",i);
        struct llnode* trail;
        struct llnode* iter = hm->map[i];
        while(iter != NULL && iter !=0)
        {
          trail = iter;
          iter = iter->next;
          free(trail);
        }
        if(iter == NULL)
        {
          printf("iter is null\n");
        }
    }
    printf("destroying map and hm");
    free(hm->map);
    free(hm);
}
void hm_remove(struct hashmap* hm, char* word, char* document_id)
{
  int bucket = hash(hm,word,document_id);
  struct llnode* headBucket = hm->map[bucket];
  if(headBucket == NULL)
  {
    printf("key value pair not found\n");
  }
  struct llnode* trail = headBucket;
  struct llnode* iter = headBucket->next;
  //removing the only node in a bucket...
  if(iter == NULL && strcmp(trail->word,word)==0 && strcmp(trail->document_id,document_id)==0)
  {
    hm->map[bucket]=NULL;
    free(headBucket);
    hm->num_elements--;
    return;
  }
  //if the node to be deleted is a head node (1st in the bucket)...
  else if(strcmp(trail->word,word)==0 && strcmp(trail->document_id,document_id)==0)
  {
    trail->next = NULL;
    free(trail);
    hm->map[bucket] = iter;
    hm->num_elements--;
    return;
  }
  //checking the rest of the list to find matching key value pair
  while(iter != NULL)
  {
    if(strcmp(iter->word,word)==0 && strcmp(iter->document_id,document_id)==0)
    {
      trail->next = iter->next;
      iter->next = NULL;
      free(iter);
      hm->num_elements--;
      return;
    }
    else{
      iter = iter->next;
      trail = trail->next;
    }
  }
  printf("reached end of list, key value pair not found.\n");
}
