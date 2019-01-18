#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/****
  Hash table key/value pair with linked list pointer
 ****/
typedef struct LinkedPair {
  char *key;
  char *value;
  struct LinkedPair *next;
} LinkedPair;

/****
  Hash table with linked pairs
 ****/
typedef struct HashTable {
  int capacity;
  LinkedPair **storage;
} HashTable;

/****
  Create a key/value linked pair to be stored in the hash table.
 ****/
LinkedPair *create_pair(char *key, char *value)
{
  LinkedPair *pair = malloc(sizeof(LinkedPair));
  pair->key = strdup(key);
  pair->value = strdup(value);
  pair->next = NULL;

  return pair;
}

/****
  Use this function to safely destroy a hashtable pair.
 ****/
void destroy_pair(LinkedPair *pair)
{
  if (pair != NULL) {
    free(pair->key);
    free(pair->value);
    free(pair);
  }
}

/****
  djb2 hash function

  Do not modify this!
 ****/
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

/****
  Fill this in.

  All values in storage should be initialized to NULL
 ****/
HashTable *create_hash_table(int capacity)
{
  HashTable *ht = malloc(sizeof(HashTable));
  ht->capacity = capacity;
  ht->storage = calloc(capacity, sizeof(LinkedPair *));

  return ht;
}

/****
  Fill this in.

  Inserting values to the same index with different keys should be
  added to the corresponding LinkedPair list.

  Inserting values to the same index with existing keys can overwrite
  the value in th existing LinkedPair list.
 ****/
void hash_table_insert(HashTable *ht, char *key, char *value)
{  
  // 1. Hash key
  unsigned int hash_key = hash(key, ht->capacity);

  // 2. Check IF the bucket at that index is occupied
  // --> if (ht->storage[index] != NULL)
  LinkedPair *current_pair = ht->storage[hash_key];
  LinkedPair *last_pair;
  

  // 4. IF OCCUPIED --> walk through the LinkedPairs to see if you find:
    // - A pair with the same key        
  while (current_pair != NULL && strcmp( current_pair->key, key ) != 0) {
    last_pair = current_pair; // --> setting last pair to the current pair
    current_pair = last_pair->next; // --> setting current pair to tail of the last pair
    // this will continue until @ end of linked list or our keys match
      // aka our current pair is holding a pair that matches
  }
    
  if (current_pair != NULL) {
    // --> if true: overwrite value
    current_pair->value = value;
  } else {
    // --> if false: create new pair and add to LinkedList
    // 3. IF NOT occupied --> add a new LinkedPair to bucket
    LinkedPair *new_pair = create_pair(key, value);
    new_pair->next = ht->storage[hash_key];
    ht->storage[hash_key] = new_pair;
  }  
  
}

/****
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys and remove matching LinkedPairs safely.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_remove(HashTable *ht, char *key)
{
  // 1. Get hash
  unsigned int hash_key = hash(key, ht->capacity);
  LinkedPair *current_pair = ht->storage[hash_key];  

  if (current_pair == NULL) {
    printf("Given key is invalid ( invalid key error )");
    exit(1);
  } 
  
  if ( strcmp(current_pair->key, key) == 0 ) { // --> If keys match, destroy pair, but set the index to the next value
    ht->storage[hash_key] = current_pair->next;
    destroy_pair(current_pair);
  } else {
    while ( current_pair->next != NULL ) { // --> Lets traverse the memory
      if ( strcmp(current_pair->next->key, key) == 0 ) { // --> if the next element's key matches:
        current_pair->next = current_pair->next->next; // --> We set the next pointer to the one after that (1->2->3) --> (1->3)
        destroy_pair(current_pair->next); // --> Continuation of last line: destroy "2"
      }
      current_pair = current_pair->next; // --> Iteration of the while loop
    }
  }



  
  
}

/****
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys.

  Return NULL if the key is not found.
 ****/
char *hash_table_retrieve(HashTable *ht, char *key)
{
  // 1. Get hash && point to ht->storage[key]
  unsigned int hash_key = hash(key, ht->capacity);
  LinkedPair *current_index = ht->storage[hash_key];


  while (current_index != 0) {
    if ( strcmp(current_index->key, key) == 0 ) { // --> Case for matching keys
      return current_index->value;
    } else if ( strcmp(current_index->key, key) != 0 && current_index->next == 0 ) { // --> Case for no matching keys & no node after ( aka end of list )
      return NULL;
    }
    // Here we want to move down the storage array ( iterate )
    current_index = current_index->next;
  }
  return 0;
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(HashTable *ht)
{
  // 1. Grab all pairs in storage and destroy the pairs
  for (int i = 0; i < ht->capacity; i++) {
    ht->storage[i] = NULL; // NOTE: this solved our `pointer being freed not allocated error
    destroy_pair(ht->storage[i]);    
  }

  // 2. Free storage -> Free hash table
  free(ht->storage);
  free(ht);
}

/****
  Fill this in.

  Should create a new hash table with double the capacity
  of the original and copy all elements into the new hash table.

  Don't forget to free any malloc'ed memory!
 ****/
HashTable *hash_table_resize(HashTable *ht)
{
  // 1. Create new hash table with double capacity
  HashTable *new_ht = create_hash_table(ht->capacity * 2);

  // 2. Copy elements into new storage
  for (int i = 0; i < ht->capacity; i++) {
    new_ht->storage[i] = ht->storage[i];
  }     
  
  // 3. Destroy old hash table
  destroy_hash_table(ht);
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

  int old_capacity = ht->capacity;
  ht = hash_table_resize(ht);
  int new_capacity = ht->capacity;

  printf("\nResizing hash table from %d to %d.\n", old_capacity, new_capacity);

  destroy_hash_table(ht);

  return 0;
}
#endif
