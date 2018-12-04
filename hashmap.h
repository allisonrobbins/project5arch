#ifndef HASHMAP_H
#define HASHMAP_H

struct llnode {
    char* word;
    int D1count;
    int D2count;
    int D3count;
    struct llnode* next;
};

struct hashmap {
    struct llnode** map;
    int num_buckets;
    int num_elements;
};

struct hashmap* hm_create(int num_buckets);
void hm_initBucket(struct hashmap* hm, int index);
struct llnode* hm_get(struct hashmap* hm, char* word);
void hm_put(struct hashmap* hm, char* word, int D1, int D2, int D3);
void hm_destroy(struct hashmap* hm);
int hash(struct hashmap* hm, char* word);
void hm_remove(struct hashmap* hm, char* word);

#endif
