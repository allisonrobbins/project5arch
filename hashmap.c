#include "hashmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct hashmap* hm_create(int num_buckets)
{
    struct hashmap* hm = (struct hashmap*)malloc(sizeof(struct hashmap));
    hm->map = (struct llnode**)malloc(sizeof(struct llnode*)*num_buckets);
    int i;
    for(i=0; i<num_buckets; i++)
    {
      hm->map[i] = (struct llnode*)malloc(sizeof(struct llnode));
    }
    hm->num_buckets = num_buckets;
    hm->num_elements = 0;
    return hm;
}
struct llnode* hm_get(struct hashmap* hm, char* word)
{
    int bucket = hash(hm,word);
    struct llnode* head = hm->map[bucket];
    struct llnode* iter = head;
    if (iter == NULL)
    {
        return iter;//return a null node
    }
    while(iter->next != NULL)
    {
        if(strcmp(iter->word, word)==0)
        {
            return iter;
        }
    }
    if(strcmp(iter->word, word)==0)
    {
        return iter;
    }
    else
    {
        return iter;//head is null, so if the word is not found, return a null node
    }
}

void hm_put(struct hashmap* hm, char* word, int D1, int D2, int D3)
{
    struct llnode* test = hm_get(hm,word);
    printf("searching for help returned node %s\n",test->word);
    if(test->word == NULL)
    {
      test->word = word;
      test->D1count = D1;
      test->D2count = D2;
      test->D3count = D3;
      test->next = NULL;
      hm->num_elements++;
      return;
    }
    while(test->next != NULL)
    {
        if(strcmp(test->word, word)==0)
        {
          test->D1count += D1;
          test->D2count += D2;
          test->D3count += D3;
          return;
        }
    }
}
int hash(struct hashmap* hm, char* word)
{
    char* a;
    int i;
    int sum = 0;
    for(a = word; *a!='\0'; a++)
    {
        i = (int) *a;
        sum = sum + i;
    }
    sum = sum%hm->num_buckets;
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
void hm_remove(struct hashmap* hm, char* word)
 {
 int bucket = hash(hm,word);
 struct llnode* headBucket = hm->map[bucket];
 if(headBucket == NULL)
 {
     printf("key value pair not found\n");
 }
 struct llnode* trail = headBucket;
 struct llnode* iter = headBucket->next;
 //removing the only node in a bucket...
 if(iter == NULL && strcmp(trail->word,word)==0)
 {
     hm->map[bucket]=NULL;
     free(headBucket);
     hm->num_elements--;
     return;
 }
 //if the node to be deleted is a head node (1st in the bucket)...
 else if(strcmp(trail->word,word)==0)
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
     if(strcmp(iter->word,word)==0)
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
