#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/****
  Basic hash table key/value pair
 ****/
typedef struct Pair {
  char *key;
  char *value;
} Pair;

/****
  Basic hash table
 ****/
typedef struct BasicHashTable {
  int capacity;
  Pair **storage;
} BasicHashTable;

/****
  Create a key/value pair to be stored in the hash table.
 ****/
Pair *create_pair(char *key, char *value)
{
  Pair *pair = malloc(sizeof(Pair));
  pair->key = strdup(key);
  pair->value = strdup(value);

  return pair;
}

/****
  Use this function to safely destroy a hashtable pair.
 ****/
void destroy_pair(Pair *pair)
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
  (hint: look up `calloc`)
 ****/
BasicHashTable *create_hash_table(int capacity)
{
  BasicHashTable *ht = malloc(sizeof(BasicHashTable)); // allocate memory for a hash table

  ht->capacity = capacity;
  ht->storage = calloc(capacity, sizeof(char *)); // allocate the memory storage for size * and fill with spaces with NULL

  return ht;
}

/****
  Fill this in.

  If you are overwriting a value with a different key, print a warning.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_insert(BasicHashTable *ht, char *key, char *value)
{
  Pair *new_pair = create_pair(key, value); // creates a new node for insertion
  unsigned int target_index = hash(key, ht->capacity); // creates an array index for node to be inserted into

  if (ht->storage[target_index] != NULL){ // If the node at the index is not empty
    fprintf(stderr, "You're overwriting an existing value value!\n");
    destroy_pair(ht->storage[target_index]); // free up the memory at that index
    ht->storage[target_index] = new_pair; // insert new node into target index
  }
  else {
    ht->storage[target_index] = new_pair; // just insert new node into target index if it's empty
  }
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_remove(BasicHashTable *ht, char *key)
{
  unsigned int hashed_key = hash(key, ht->capacity); // hash the incoming key to find the index

  if (ht->storage[hashed_key] == NULL) { // if the index at storage is empty, return a message
    fprintf(stderr, "Unable to remove entry with provided key.");
  }

  ht->storage[hashed_key] = NULL; // otherwise set the current node to NULL
}

/****
  Fill this in.

  Should return NULL if the key is not found.
 ****/
char *hash_table_retrieve(BasicHashTable *ht, char *key)
{
  unsigned int hashed_key = hash(key, ht->capacity); // hash the incoming key to find the index

  if (ht->storage[hashed_key] != NULL) { // if the index at storage is not empty
    return ht->storage[hashed_key]->value; // return value at key
  }

  return NULL; // if nothing is there, return null
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(BasicHashTable *ht)
{
  for (int i=0; i < ht->capacity; i++) {
    ht->storage[i] = NULL; // without NULL -> error for object 0x7fbecec02b20: pointer being freed was not allocated
    destroy_pair(ht->storage[i]); // destroy pair has built in NULL check and frees key, value, and node
  }
  free(ht->storage);
  free(ht);
}


#ifndef TESTING
int main(void)
{
  struct BasicHashTable *ht = create_hash_table(16);

  // MY TESTS
  // hash_table_insert(ht, "tim", "texas\n"); // index 15
  // hash_table_insert(ht, "josh", "mexico\n"); // index 9
  // hash_table_insert(ht, "alex", "missouri\n"); // also index 15, collision -> will override tim
  // printf("%s", hash_table_retrieve(ht, "josh")); // returns mexico
  // hash_table_remove(ht, "josh"); // removes the specified key
  // printf("%s", hash_table_retrieve(ht, "josh")); // returns nothing since it was deleted
  // destroy_hash_table(ht);

  // DEFAULT TESTS
  hash_table_insert(ht, "line", "Here today...\n");

  printf("%s", hash_table_retrieve(ht, "line"));

  hash_table_remove(ht, "line");

  if (hash_table_retrieve(ht, "line") == NULL) {
    printf("...gone tomorrow. (success)\n");
  } else {
    fprintf(stderr, "ERROR: STILL HERE\n");
  }

  destroy_hash_table(ht);

  return 0;
}
#endif
