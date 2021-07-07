#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct LinkedPair {
  char *key;
  char *value;
  struct LinkedPair *next;
} LinkedPair;

typedef struct HashTable {
  int capacity;
  LinkedPair **storage;
} HashTable;

LinkedPair *create_pair(char *key, char *value)
{
  LinkedPair *pair = malloc(sizeof(LinkedPair));
  pair->key = strdup(key);
  pair->value = strdup(value);
  pair->next = NULL;

  return pair;
}

void destroy_pair(LinkedPair *pair)
{
  if (pair != NULL) {
    pair->key = NULL;
    pair->value = NULL;
    free(pair->key);
    free(pair->value);
    free(pair);
  }
}

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

HashTable *create_hash_table(int capacity)
{
  HashTable *ht = malloc(sizeof(HashTable)); // allocate memory for a hash table;

  ht->capacity = capacity;
  ht->storage = calloc(capacity, sizeof(LinkedPair *)); // allocate the memory storage for a struct LinkedPair pointer;

  return ht;
}

void hash_table_insert(HashTable *ht, char *key, char *value)
{
  unsigned int target_index = hash(key, ht->capacity); // creates an array index for node to be inserted into
  LinkedPair *new_pair = create_pair(key, value); // creates a new node for insertion

  if (ht->storage[target_index] == 0) { // if the current index is empty, insert new_pair
    ht->storage[target_index] = new_pair;
  }
  else {
    while (ht->storage[target_index] != 0) {
      if (strcmp(ht->storage[target_index]->key, key) == 0) { // == or != compares base addresses, strcmp to compare values
        ht->storage[target_index]->value = value;
        break;
      }
      else if (strcmp(ht->storage[target_index]->key, key) != 0 && ht->storage[target_index]->next == NULL) {
        ht->storage[target_index]->next = new_pair;
        break;
      }
      ht->storage[target_index] = ht->storage[target_index]->next;
    }
  }
}

void hash_table_remove(HashTable *ht, char *key)
{
  unsigned int target_index = hash(key, ht->capacity);
  LinkedPair *current = ht->storage[target_index]; // shortens the name

  while (current != 0) { // LOOP while there's a current node
    if (strcmp(current->key, key) == 0 && current->next == 0) { // EXIT 1 if strings are the same and there's NOT a linked node
      destroy_pair(current);
      break;
    }
    else if (strcmp(current->key, key) == 0 && current->next != 0) { // EXIT 2 if strings are same and there IS a linked node
      current->key = current->next->key;
      current->value = current->next->value;
      current->next = current->next->next;
      break;
    }
    current = current->next; // ITERATOR Move to the next linked node and continue with while loop to check
  }
}

char *hash_table_retrieve(HashTable *ht, char *key)
{
  unsigned int target_index = hash(key, ht->capacity);
  LinkedPair *current = ht->storage[target_index]; // shortens the name and serve as marker

  while (current != 0) { // LOOP while there's a current node
    if (strcmp(current->key, key) == 0) { // EXIT 1 if keys match, return the value
      return current->value;
    }
    else if (strcmp(current->key, key) != 0 && current->next == 0) { // EXIT 2 if no key matches and no linked node
      fprintf(stderr, "That key was not found.");
      return NULL;
    }
    current = current->next; // ITERATOR Move to the next linked node and continue with while loop to check
  }
  return 0;
}

void destroy_hash_table(HashTable *ht)
{
  for (int i = 0; i < ht->capacity; i++) {
    ht->storage[i] = NULL; // without NULL -> error for object 0x7fbecec02b20: pointer being freed was not allocated
    destroy_pair(ht->storage[i]); // destroy pair has built in NULL check and frees key, value, and node
  }

  free(ht->storage);
  free(ht);
}

HashTable *hash_table_resize(HashTable *ht)
{
  HashTable *new_ht = create_hash_table(ht->capacity*2);

  for (int i = 0; i < ht->capacity; i++) {
    new_ht->storage[i] = ht->storage[i];
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
