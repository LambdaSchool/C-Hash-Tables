#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  Hash table key/value pair with linked list pointer.

  Note that an instance of `LinkedPair` is also a node in a linked list.
  More specifically, the `next` field is a pointer pointing to the the 
  next `LinkedPair` in the list of `LinkedPair` nodes. 
 */
typedef struct LinkedPair {
  char *key;
  char *value;
  struct LinkedPair *next;
} LinkedPair;

/*
  Hash table with linked pairs.
 */
typedef struct HashTable {
  int capacity;
  LinkedPair **storage;
} HashTable;

/*
  Create a key/value linked pair to be stored in the hash table.
 */
LinkedPair *create_pair(char *key, char *value)
{
  LinkedPair *pair = malloc(sizeof(LinkedPair));
  pair->key = strdup(key);
  pair->value = strdup(value);
  pair->next = NULL;

  return pair;
}

/*
  Use this function to safely destroy a hashtable pair.
 */
void destroy_pair(LinkedPair *pair)
{
  if (pair != NULL) {
    free(pair->key);
    free(pair->value);
    free(pair);
  }
}

/*
  djb2 hash function

  Do not modify this!
 */
unsigned int hash(char *str, int max)
{
  unsigned long hash = 5381;
  int c;
  unsigned char * u_str = (unsigned char *)str;

  while ((c = *u_str++)) {
    hash = ((hash << 5) + hash) + c;
  }

  return hash % max;
}

/*
  Fill this in.

  All values in storage should be initialized to NULL
 */
HashTable *create_hash_table(int capacity)
{
  HashTable *ht;
  ht = malloc(sizeof(struct HashTable));
  ht->storage = malloc(capacity*sizeof(struct LinkedPair*));
  for(int i=0; i<capacity; i++)
  {
    ht->storage[i] = NULL;
  }
  ht->capacity = capacity;
  return ht;
}

/*
  Fill this in.

  Inserting values to the same index with different keys should be
  added to the corresponding LinkedPair list.

  Inserting values to the same index with existing keys can overwrite
  the value in th existing LinkedPair list.
 */
void hash_table_insert(HashTable *ht, char *key, char *value)
{
  LinkedPair **p = &ht->storage[hash(key, ht->capacity)];
  while(*p != NULL)
  {
    if(strcmp(key, (*p)->key))
    {
      p = &(*p)->next;
    }
    else
    {
      destroy_pair(*p);
      break;
    }
  }
  *p = create_pair(key, value);
}

/*
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys and remove matching LinkedPairs safely.

  Don't forget to free any malloc'ed memory!
 */
void hash_table_remove(HashTable *ht, char *key)
{
  int index = hash(key, ht->capacity);
  LinkedPair *p = ht->storage[index];
  if(p == NULL){
    printf("%s not found\n", key);
    return;
  }
  int first = 1;
  LinkedPair *prev;
  while(strcmp(p->key, key)){
    first = 0;
    prev = p;
    p = p->next;
  }
  if(first){
    ht->storage[index] = p->next;
  }else{
    prev->next = p->next;
  }
  destroy_pair(p);
}

/*
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys.

  Return NULL if the key is not found.
 */
char *hash_table_retrieve(HashTable *ht, char *key)
{
  LinkedPair *p = ht->storage[hash(key, ht->capacity)];
  if(p == NULL)
  {
    return NULL;
  }
  while(strcmp(p->key, key))
  {
    p = p->next;
    if(p == NULL)
    {
      return NULL;
    }
  }
  return p->value;
}

/*
  Fill this in.

  Don't forget to free any malloc'ed memory!
 */
void destroy_hash_table(HashTable *ht)
{
  for(int i=0; i<ht->capacity; i++)
  {
    if(ht->storage[i] == NULL)
    {
      continue;
    }
    LinkedPair *p = ht->storage[i];
    LinkedPair *p_next = ht->storage[i]->next;
    while(p_next != NULL)
    {
      destroy_pair(p);
      p = p_next;
      p_next = p->next;
    }
    destroy_pair(p);
  }
  free(ht->storage);
  free(ht);
}

/*
  Fill this in.

  Should create a new hash table with double the capacity
  of the original and copy all elements into the new hash table.

  Don't forget to free any malloc'ed memory!
 */
HashTable *hash_table_resize(HashTable *ht)
{
  HashTable *new_ht;
  int new_capacity = ht->capacity*2;
  new_ht = create_hash_table(new_capacity);
  for(int i=0; i<ht->capacity; i++)
  {
    if(ht->storage[i] == NULL)
    {
      continue;
    }
    LinkedPair *p = ht->storage[i];
    LinkedPair *p_next = ht->storage[i]->next;
    while(p_next != NULL)
    {
      hash_table_insert(new_ht, p->key, p->value);
      destroy_pair(p);
      p = p_next;
      p_next = p->next;
    }
    destroy_pair(p);
    hash_table_insert(new_ht, p->key, p->value);
  }
  free(ht->storage);
  free(ht);
  return new_ht;
}


#ifndef TESTING
int main(void)
{
  struct HashTable *ht = create_hash_table(2);

  hash_table_insert(ht, "line_1", "Tiny hash table\n");
  hash_table_insert(ht, "line_2", "Filled beyond capacity\n");
  hash_table_insert(ht, "line_3", "Linked list saves the day!\n");

  printf("%s", hash_table_retrieve(ht, "line_1"));
  printf("%s", hash_table_retrieve(ht, "line_2"));
  printf("%s", hash_table_retrieve(ht, "line_3"));

  hash_table_remove(ht, "line_1");
  printf("remove success\n");
  printf("%s", hash_table_retrieve(ht, "line_1"));

  int old_capacity = ht->capacity;
  ht = hash_table_resize(ht);
  int new_capacity = ht->capacity;

  printf("\nResizing hash table from %d to %d.\n", old_capacity, new_capacity);

  destroy_hash_table(ht);

  return 0;
}
#endif
