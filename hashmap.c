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
    return hm;
}
int hm_get(struct hashmap* hm, char* word, char* document_id)
{
  //printf("Made it to get method\n");  
  int bucket = hash(hm,word,document_id);
    struct llnode* top = hm->map[bucket];
    if(top == NULL)
    {
      printf("there is nothing in bucket %i\n",bucket);
      printf("the number of occurrences for %s %s is 0. Returning -1.\n",word,document_id);
      return -1;
    }
    //printf("bucket %i is not empty\n",bucket);
    struct llnode* iter = top;
    while(iter->next != NULL)
    {
      if(strcmp(iter->document_id,document_id)==0 && strcmp(iter->word,word)==0)
      {
        printf("the number of occurrences for %s %s is %i\n",word,document_id,iter->num_occurrences);  
        return iter->num_occurrences;//if they match, return the number of occurrences
      }
      iter = iter->next;
    }
    if(strcmp(iter->document_id,document_id)==0 && strcmp(iter->word,word)==0)
      {
        printf("the number of occurrences for %s %s is %i\n",word,document_id,iter->num_occurrences);  
        return iter->num_occurrences;//if they match, return the number of occurrences
      }
    printf("the number of occurrences for %s %s is 0. Returning -1.\n",word,document_id);
    return -1;
}
/* This method takes a given word-document pair and updates the num_occurrences if it exists
*  or it creates a new llnode for that pair and inputs the num_occurrences there
*/
void hm_put(struct hashmap* hm, char* word, char* document_id, int num_occurrences){
    int bucket = hash(hm,word,document_id);
    struct llnode *headBucket = hm->map[bucket];
  //printf("head node is %s %s\n", headBucket->word,headBucket->document_id);
    if (headBucket==NULL) 
  {
        printf("nothing in the bucket yet\n");
        struct llnode* new_node = (struct llnode*) calloc(1,sizeof(struct llnode));
        printf("allocated space for new node\n");
        hm->map[bucket] = new_node;
        new_node->word = word;
        new_node->document_id = document_id;
        new_node->num_occurrences = num_occurrences;
        printf("Adding word: %s doc: %s to bucket %i\n",new_node->word,new_node->document_id,bucket);
        hm->num_elements += 1;
    }
    else {
        struct llnode* curr = headBucket;
        struct llnode* prev = headBucket;
        while (curr != 0) {
            if (strcmp(curr->word, word) == 0 && strcmp(curr->document_id, document_id) == 0) {
                curr->num_occurrences += num_occurrences;
                printf("after update, number of occur in hm_put: %s %d\n", word, curr->num_occurrences);
                return;
            }
            prev = curr;
            curr = curr->next;
        }
        /* Add node to end of list */
    printf("adding to the end of bucket %i\n",bucket);
        struct llnode* new_node = (struct llnode*) calloc(1,sizeof(struct llnode));
        new_node->word = word;
        new_node->document_id = document_id;
        new_node->num_occurrences = num_occurrences;
        prev->next = new_node;
        hm->num_elements += 1;
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
