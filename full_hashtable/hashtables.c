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
    pair->key = NULL;
    pair->value = NULL;
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
  HashTable *ht = malloc(sizeof(HashTable)); // allocate memory for a hash table;

  ht->capacity = capacity;
  ht->storage = calloc(capacity, sizeof(LinkedPair *)); // allocate the memory storage for a struct LinkedPair pointer;

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
  unsigned int target_index = hash(key, ht->capacity);
  LinkedPair *new_pair = create_pair(key, value);
  LinkedPair *current = ht->storage[target_index];
  
  if (ht->storage[target_index] == NULL) { // if the current index is empty, insert new_pair
    ht->storage[target_index] = new_pair;
  }
  else {
    while (current != NULL) { // if the current index is not empty, 3 things can happens
      // 1. if the keys are the same, then overwrite existing value with the new value
      if (strcmp(current->key, key) == 0) { // == or != compares base addresses, strcmp to compare values
        current->value = value;
        break;
      }
      // 2. if there's an empty "next" slot, insert new pair there
      if (current->next == NULL) {
        current->next = new_pair;
        break;
      }
      // 3. If neither conditionals catches, must mean that it's a new linked list node
      // continue with while loop until there are matching keys or there's an empty "next" slot
      current = current->next;
    }
  }
}

/*
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys and remove matching LinkedPairs safely.

  Don't forget to free any malloc'ed memory!
 */
void hash_table_remove(HashTable *ht, char *key)
{
  unsigned int target_index = hash(key, ht->capacity);
  LinkedPair *current = ht->storage[target_index];
  LinkedPair *previous = NULL;

  while (current != NULL && strcmp(current->key, key) != 0) { // traverses until gets to last node or an equal key
    previous = current;
    current = current->next;
  }

  if (previous == NULL) { // if it's the first and only node
    ht->storage[target_index] = current->next;
  }
  else { // unlinks the current node then stitch the ends or also removes last node from list
    previous->next = current->next;
  }

  // while (current != NULL && strcmp(current->key, key) != 0) { // LOOP while there's a current node
  //   // if (strcmp(current->key, key) == 0 && current->next == NULL) { // EXIT 1 if strings are the same and there's NOT a linked node
  //   //   printf("line 135");
  //   //   destroy_pair(current);
  //   //   break;
  //   // }
  //   previous = current;
  //   current = current->next;

  //   // if (strcmp(current->key, key) == 0 && current->next != NULL) { // EXIT 2 if strings are same and there IS a linked node
  //   //   printf("line 139");
  //   //   previous = current;
  //   //   current = current->next;

  //   //   // current->value = current->next->value;
  //   //   // current->next = current->next->next;
  //   //   break;
  //   // }
  // // destroy_pair(previous);
  //   // current = current->next; // ITERATOR Move to the next linked node and continue with while loop to check
  // }

  // if (previous == NULL) {  // Removing the first element in the Linked List
  //   ht->storage[target_index] = current->next;
  // } else {
  //   previous->next = current->next;
  // }
}

/*
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys.

  Return NULL if the key is not found.
 */
char *hash_table_retrieve(HashTable *ht, char *key)
{
  unsigned int target_index = hash(key, ht->capacity);
  LinkedPair *current = ht->storage[target_index];

  while (current != NULL) { // LOOP while there's a current node
    if (strcmp(current->key, key) == 0) { // EXIT 1 if keys match, return the value
      return current->value;
    }
    current = current->next; // ITERATOR if no match, check at the next node
  }

  return NULL;

  // while (current != NULL) { // LOOP while there's a current node
  //   if (strcmp(current->key, key) == 0) { // EXIT 1 if keys match, return the value
  //     return current->value;
  //   }
  //   // else if (strcmp(current->key, key) != 0 && current->next == NULL) { // EXIT 2 if no key matches and no linked node, return NULL
  //   //   return NULL;
  //   // }
  //   current = current->next; // ITERATOR Move to the next linked node and continue with while loop to check
  // }
  // return NULL;
}

/*
  Fill this in.

  Don't forget to free any malloc'ed memory!
 */
void destroy_hash_table(HashTable *ht)
{
  for (int i = 0; i < ht->capacity; i++) {
    ht->storage[i] = NULL; // without NULL -> error for object 0x7fbecec02b20: pointer being freed was not allocated
    destroy_pair(ht->storage[i]); // destroy pair has built in NULL check and frees key, value, and node
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
  HashTable *new_ht = create_hash_table(ht->capacity*2); // Create new hash table with double capacity

  for (int i = 0; i < ht->capacity; i++) { // copy from old storage to new storage
    new_ht->storage[i] = ht->storage[i];
  }

  destroy_hash_table(ht); // destroy old hash table

  return new_ht;
}


#ifndef TESTING
int main(void)
{
  struct HashTable *ht = create_hash_table(2);

  // DEFAULT
  // hash_table_insert(ht, "line_1", "Tiny hash table\n");
  // hash_table_insert(ht, "line_2", "Filled beyond capacity\n");
  // hash_table_insert(ht, "line_3", "Linked list saves the day!\n");
  // printf("%s", hash_table_retrieve(ht, "line_1"));
  // printf("%s", hash_table_retrieve(ht, "line_2"));
  // printf("%s", hash_table_retrieve(ht, "line_3"));
  // int old_capacity = ht->capacity;
  // ht = hash_table_resize(ht);
  // int new_capacity = ht->capacity;
  // printf("\nResizing hash table from %d to %d.\n", old_capacity, new_capacity);

  // // MY TESTS
  // hash_table_insert(ht, "tim", "texas\n"); // inserts index 1
  // printf("%s\n", hash_table_retrieve(ht, "tim")); // returns texas
  // hash_table_insert(ht, "josh", "mexico\n"); // inserts index 1 -> INDEX COLLISION, sets on existing node's "next"
  // printf("%s\n", hash_table_retrieve(ht, "josh")); // returns mexico
  // hash_table_insert(ht, "tim", "california\n"); // inserts index 1 -> INDEX & KEY COLLISION, set on exisiting node's "value"
  // printf("%s\n", hash_table_retrieve(ht, "tim")); // returns california
  // // hash_table_remove(ht, "tim"); // removes tim from the 1st node and shift josh to 1st node
  // // hash_table_remove(ht, "josh"); // removes josh from the 2nd node

  // SYSTEM TESTS
  // struct HashTable *ht = create_hash_table(8);
  // char *return_value = hash_table_retrieve(ht, "key-0");
  // printf("%s\n", return_value);
  // mu_assert(return_value == NULL, "Initialized value is not NULL");
  // hash_table_insert(ht, "key-0", "val-0");
  // hash_table_insert(ht, "key-1", "val-1");
  // hash_table_insert(ht, "key-2", "val-2");
  // hash_table_insert(ht, "key-3", "val-3");
  // hash_table_insert(ht, "key-4", "val-4");
  // hash_table_insert(ht, "key-5", "val-5");
  // hash_table_insert(ht, "key-6", "val-6");
  // hash_table_insert(ht, "key-7", "val-7");
  // hash_table_insert(ht, "key-8", "val-8");
  // hash_table_insert(ht, "key-9", "val-9");
  // hash_table_insert(ht, "key-0", "new-val-0");
  // hash_table_insert(ht, "key-1", "new-val-1");
  // hash_table_insert(ht, "key-2", "new-val-2");
  // hash_table_insert(ht, "key-3", "new-val-3");
  // hash_table_insert(ht, "key-4", "new-val-4");
  // hash_table_insert(ht, "key-5", "new-val-5");
  // hash_table_insert(ht, "key-6", "new-val-6");
  // hash_table_insert(ht, "key-7", "new-val-7");
  // hash_table_insert(ht, "key-8", "new-val-8");
  // hash_table_insert(ht, "key-9", "new-val-9");
  // hash_table_remove(ht, "key-9");
  // hash_table_remove(ht, "key-8");
  // hash_table_remove(ht, "key-7");
  // hash_table_remove(ht, "key-6");
  // hash_table_remove(ht, "key-5");
  // hash_table_remove(ht, "key-4");
  // hash_table_remove(ht, "key-3");
  // hash_table_remove(ht, "key-2");
  // hash_table_remove(ht, "key-1");
  // hash_table_remove(ht, "key-0");
  // return_value = hash_table_retrieve(ht, "key-0");
  // printf("%s\n", return_value);

  return 0;
}
#endif
