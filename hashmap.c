#include "hashmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct hashmap* hm_create(int num_buckets)//this method creates the hashmap!
{
    struct hashmap* hm = (struct hashmap*)malloc(sizeof(struct hashmap));//first, alocate for the hashmap struct
    hm->map = (struct llnode**)malloc(sizeof(struct llnode*)*num_buckets);//next, allocate map for number of buckets
    int i;
    for(i=0; i<num_buckets; i++)//iterate through map
    {
      hm->map[i] = (struct llnode*)malloc(sizeof(struct llnode));//allocate for each llnode
      hm->map[i]->word = "null";
      /*so I was getting a lot of weird errors when trying to find null buckets, so I initialized the head of each
      bucket to have the head's word be "null" and that made it easier to test for a null node*/
    }
    hm->num_buckets = num_buckets;//set these variables
    hm->num_elements = 0;//nothings there yet
    return hm;
}
struct llnode* hm_get(struct hashmap* hm, char* word)
{
  int bucket = hash(hm,word);//find which bucket this node should be in
  struct llnode* head = hm->map[bucket];
  struct llnode* iter = head;
  if (strcmp(iter->word,"null")==0)//if the head of the bucket is null, then the word is not in the hashmap
  {
      return iter;//return a null node
  }
  if(strcmp(iter->word, word)==0)//if the word matches the one we are looking for, then return that node
  {
      return iter;
  }
  while(iter->next!=NULL)
  {
    if(strcmp(iter->word, word)==0)//if the word matches the one we are looking for, then return that node
    {
        return iter;
    }
    iter = iter->next;//iterate through linked list
  }
      return iter;//that word is not in the hashmap so return whatever node we are at
}
  void hm_put(struct hashmap* hm, char* word, int D1, int D2, int D3)//putting a word in the hashmap!
	{
	    int bucket = hash(hm,word);//find which bucket the node should go in
	    struct llnode* iter = hm->map[bucket];
	    if(strcmp(iter->word,"null")==0)//if the head of the bucket is null, put the word in there
	    {
	      iter->word = word;
	      iter->D1count = D1;
	      iter->D2count = D2;
	      iter->D3count = D3;
	      iter->next = NULL;
	      hm->num_elements++;
	      return;
	    }
	    while(iter->word!=NULL)//otherwise, iterate through the list and look for a match
	    {
	        if(strcmp(iter->word, word)==0)//if that is the right word...
	        {
	          iter->D1count += D1;//increment document counters
	          iter->D2count += D2;
	          iter->D3count += D3;
	          return;
	        }
	        if(iter->next == NULL)//if nothing comes next, then add a node after
	        {
	          iter->next=(struct llnode*)malloc(sizeof(struct llnode));//need to allocate memory for the node
	          iter = iter->next;
	          iter->word = word;//set all the values
	          iter->D1count = D1;
	          iter->D2count = D2;
            iter->D3count = D3;
	          iter->next = NULL;
	          hm->num_elements++;//increment this
	          return;
	        }
	        iter = iter->next;//otherwise, iterate
	    }
	}
int hash(struct hashmap* hm, char* word)//find which bucket it should be in
{
    char* a;
    int i;
    int sum = 0;
    for(a = word; *a!='\0'; a++)//iterate through chars 1 by 1
    {
        i = (int) *a;//convert to ascii
        sum = sum + i;//add to sum
    }
    sum = sum%hm->num_buckets;//using the hash function given
    return sum;
}
void hm_destroy(struct hashmap* hm)//iterates through hashmap and destroys everything
{
    int i;
    for(i=0; i<hm->num_buckets; i++)//goes to every bucket
    {
        printf("destroying bucket %i\n",i);
        struct llnode* trail;
        struct llnode* iter = hm->map[i];
        while(iter != NULL && iter !=0)//iterate through and free all nodes
        {
            trail = iter;
            iter = iter->next;
            free(trail);
        }
    }
    printf("destroying map and hm\n");
    free(hm->map);//now, free map pointer
    free(hm);//free hashmap pointer
}
void hm_remove(struct hashmap* hm, char* word)
 {
 int bucket = hash(hm,word);
 struct llnode* deleteMe = hm_get(hm,word);
 if(strcmp(deleteMe->word,"null")==0)
 {
     printf("key value pair not found\n");
 }
 struct llnode* trail = deleteMe;
 struct llnode* iter = deleteMe->next;
 //removing the only node in a bucket...
 if(iter == NULL && strcmp(trail->word,word)==0)
 {
     hm->map[bucket]=NULL;
     free(deleteMe);
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
