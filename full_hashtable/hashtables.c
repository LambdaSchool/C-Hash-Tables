#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/****
  Hash table key/value pair with linked list pointer
 ****/
typedef struct LinkedPair
{
  char *key;
  char *value;
  struct LinkedPair *next;
} LinkedPair;

/****
  Hash table with linked pairs
 ****/
typedef struct HashTable
{
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
 ****/
HashTable *create_hash_table(int capacity)
{
  HashTable *ht;
  ht = malloc(sizeof(HashTable));
  ht->capacity = capacity;
  ht->storage = (LinkedPair **)calloc(capacity, sizeof(LinkedPair));

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
  int i;
  LinkedPair *pair;
  LinkedPair *node;

  i = hash(key, ht->capacity);
  if (ht->storage[i] != NULL)
  {
    node = ht->storage[i];
    while (node != NULL)
    {
      if (strcmp(node->key, key) == 0)
      {
        pair = create_pair(key, value);
        ht->storage[i] = pair;
        destroy_pair(node);
        break;
      }
      if (node->next == NULL)
      {
        pair = create_pair(key, value);
        node->next = pair;
        break;
      }
    }
  }
  else
  {
    pair = create_pair(key, value);
    ht->storage[i] = pair;
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
  int i;
  LinkedPair *node;
  LinkedPair *prevNode;

  i = hash(key, ht->capacity);

  if (ht->storage[i] == NULL)
  {
    printf("Nothing there\n");
  }

  if (strcmp(ht->storage[i]->key, key) == 0 && ht->storage[i]->next == NULL)
  {
    destroy_pair(ht->storage[i]);
    ht->storage[i] = NULL;
  }
  else
  {
    if (strcmp(ht->storage[i]->key, key) == 0 && ht->storage[i]->next != NULL)
    {
      node = ht->storage[i];
      ht->storage[i] = node->next;
      destroy_pair(node);
    }
    else
    {
      prevNode = ht->storage[i];
      node = ht->storage[i]->next;
      while (node != NULL)
      {
        if (strcmp(node->key, key) == 0)
        {
          if (node->next == NULL)
          {
            prevNode->next = node->next;
            destroy_pair(node);
            break;
          }
          else
          {
            prevNode->next = NULL;
            destroy_pair(node);
            break;
          }
        }
        prevNode = node;
        node = node->next;
      }
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
  int i;
  LinkedPair *node;

  i = hash(key, ht->capacity);

  if (ht->storage[i] == NULL)
  {
    return NULL;
  }

  if (strcmp(ht->storage[i]->key, key) == 0 && ht->storage[i]->next == NULL)
  {
    return ht->storage[i]->value;
  }
  else
  {
    node = ht->storage[i];
    while (node != NULL)
    {
      if (strcmp(node->key, key) == 0)
      {
        return node->value;
      }
      node = node->next;
    }
  }
  return NULL;
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(HashTable *ht)
{
  for (int i = 0; i < ht->capacity; i++)
  {
    if (ht->storage[i] != NULL)
    {
      destroy_pair(ht->storage[i]);
    }
  }
  if (ht->storage != NULL)
  {
    free(ht->storage);
  }
  if (ht != NULL)
  {
    free(ht);
  }
}

/****
  Fill this in.

  Should create a new hash table with double the capacity
  of the original and copy all elements into the new hash table.

  Don't forget to free any malloc'ed memory!
 ****/
HashTable *hash_table_resize(HashTable *ht)
{
  HashTable *new_ht;

  new_ht = create_hash_table(ht->capacity * 2);
  for (int i = 0; i < ht->capacity; i++)
  {
    LinkedPair *node = ht->storage[i];
    while (node != NULL)
    {
      hash_table_insert(new_ht, node->key, node->value);
      node = node->next;
    }
  }

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
  hash_table_insert(ht, "line_4", "Linked list saves the day!\n");
  hash_table_insert(ht, "line_5", "Linked list saves the day!\n");

  printf("%s", hash_table_retrieve(ht, "line_1"));
  printf("%s", hash_table_retrieve(ht, "line_2"));
  printf("%s", hash_table_retrieve(ht, "line_3"));
  printf("%s", hash_table_retrieve(ht, "line_4"));
  hash_table_remove(ht, "line_3");

  printf("Line_3: %s", hash_table_retrieve(ht, "line_3"));
  printf("%s", hash_table_retrieve(ht, "line_4"));

  int old_capacity = ht->capacity;
  ht = hash_table_resize(ht);
  int new_capacity = ht->capacity;

  printf("\nResizing hash table from %d to %d.\n", old_capacity, new_capacity);

  destroy_hash_table(ht);

  return 0;
}
#endif
