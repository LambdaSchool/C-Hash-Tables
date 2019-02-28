#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/****
  Basic hash table key/value pair
 ****/

typedef struct Pair
{
  char *key;
  char *value;
} Pair;

/****
  Basic hash table
 ****/

typedef struct BasicHashTable
{
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
  if (pair != NULL)
  {
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
  unsigned char *u_str = (unsigned char *)str;

  while ((c = *u_str++))
  {
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
  BasicHashTable *ht = malloc(sizeof(BasicHashTable));

  ht->capacity = capacity;
  ht->storage = calloc(capacity, sizeof(Pair *));

  return ht;
}

/****
  Fill this in.
  Don't forget to free any malloc'ed memory!
 ****/

void hash_table_insert(BasicHashTable *ht, char *key, char *value)
{
  // Unsined because we just want positive values.
  unsigned int hash_index = hash(key, ht->capacity);

  // Create new pair
  Pair *pair = create_pair(key, value);

  // Get current stored pair
  Pair *stored_pair = ht->storage[hash_index];

  // If you are overwriting a value with a different key, print a warning.
  if (stored_pair)
  {
    // If the key matches the key already in place it is not overwriting
    if (strcmp(key, stored_pair->key) != 0)
    {
      //  strcmp() returns 0 if both arguments are equal
      printf("\n Error-The key you previous set doesn't match the new key being set. Overwriting");
    }

    destroy_pair(stored_pair);
  }

  // Update the hash_index in storage
  ht->storage[hash_index] = pair;
}

/****
  Fill this in.
  Don't forget to free any malloc'ed memory!
 ****/

void hash_table_remove(BasicHashTable *ht, char *key)
{
  unsigned int hash_index = hash(key, ht->capacity);

  // check if hash_index is null, if so, print error and return
  if (ht->storage[hash_index] == NULL)
  {
    fprintf(stderr, "%s does not exists", key);
    return;
  }

  Pair *pair = ht->storage[hash_index];

  destroy_pair(pair);

  // Reset value to null to prevent garbage and return it to it's calloc'd state
  ht->storage[hash_index] = NULL;
}

/****
  Fill this in.
  Should return NULL if the key is not found.
 ****/

char *hash_table_retrieve(BasicHashTable *ht, char *key)
{
  unsigned int hash_index = hash(key, ht->capacity);

  if (ht->storage[hash_index] != NULL)
  {
    printf("Hash table exists at hash_index %d.\n", hash_index);

    return ht->storage[hash_index]->value;
  }

  return NULL;
}

/****
  Fill this in.
  Don't forget to free any malloc'ed memory!
 ****/

void destroy_hash_table(BasicHashTable *ht)
{
  if (ht == NULL)
  {
    printf("Error: No hash table provided.");
    exit(1);
  }

  for (int index = 0; index < ht->capacity; index++)
  {
    if (ht->storage[index] != NULL)
    {
      destroy_pair(ht->storage[index]);
    }
  }

  free(ht->storage);
  free(ht);
}

#ifndef TESTING

int main(void)
{
  struct BasicHashTable *ht = create_hash_table(16);

  hash_table_insert(ht, "line", "Here today...\n");

  printf("%s", hash_table_retrieve(ht, "line"));

  hash_table_remove(ht, "line");

  if (hash_table_retrieve(ht, "line") == NULL)
  {
    printf("...gone tomorrow. (success)\n");
  }
  else
  {
    fprintf(stderr, "ERROR: STILL HERE\n");
  }

  destroy_hash_table(ht);

  return 0;
}

#endif