#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/****
  Basic hash table key/value pair
  PR
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
  pair->key = key;
  pair->value = value;
  

  return pair;
}

/****
  Use this function to safely destroy a hashtable pair.
 ****/
void destroy_pair(Pair *pair)
{
  if (pair != NULL) free(pair);
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
  /*
  So before we even write code to put this stuff in, think about what properties
  does your hash table need.  array the idk if it's the key or the hashed int  but one of those maybe is what i've been thinking | a pointer to the value with the name of the hash?
  Come to the open zoom
  lame holdup lol if you can't it's ok
  lemme explain it here then
  The core data storage in a hash table is a good ol' fashioned array
  That means numerical indices and values 
  But wait, hash tables use string keys to hook up with values. How does that work?
  string key -(hash function)-> integer index
  That means you're not saving the hash. The hash is actually the string converted to
  a numerical index, which you can then use to access elements in the array.
  Does that make sense?
  Kindof
  What I get is the hash more or less is the index because it gets % down with the legnth of the array
  so each value is just a string R: You can look at the struct definition and `create_pair` and see what type the value is so it's a string in our scenerio since it's a character type pointer for the value
  so key get's turned into a numerical value, gets hashed more or less down to our array size, we then put the value at that index of the array and we don't do anything else with the key? I 
  wanted to say that we would assign the hmmm
  R: Yes. They key gets converted to a numerical index. We do nothing further with the key after we have found its converted index equivalent. gotcha 
  and calloc will initiate a Null array the size of 16 with my implementation but what is it wanting the size of. Like why do we need that arg? besides just that's what it wants.
  R: Why do we need that arg indeed. Let's look at the main function below:
  So back up here, notice it's passing the integer 16 when it creates the hash table.
  
  so if they are going to be passing in values then we don't want to hard code in 16 up here. R:yep but still the sizeof seems 
  R: OK so what is going to be stored in this calloc'd memory? 
  well it looked like strings for the key/value 
  what about char pointers to the value
  do you have a data structure that holds key value pairs up in here?
  a struct R: and what's it called? Pair
  holdup so this is an array of structs? like if you go at index[0] you'll get the pair & not just the value? R: Yessir. Oh, 
  R: so for each slot in the calloc'd memory, you need to be holding a pair. that means that each slot needs to be big enough to hold a pair
  */ 
  BasicHashTable *ht;
  ht = malloc(sizeof(BasicHashTable));
  ht->capacity = capacity; 
  ht->storage = calloc(capacity, sizeof(Pair *)); // for example, this will hold 16 numbers so that it's not a static value would it be looking for a variable there?
  // but what is it supposed to be holding. just strings?  Hint: Look at the struct definition of where this calloc is supposed to be assigned to
  // as for what it's holding, it's (going to) hold pointers to Pairs, hence `sizeof(Pair *)` as the second argument of calloc
  // R: so right now you are reassigning ht to a different block of memory. But line 104 is perfectly fine.
  // R: Remember, you have to now build out this structure you just carved some space in memory for
  /*
  typedef struct BasicHashTable {
    int capacity; // an integer representing how many indices in the array
    Pair **storage; // the array itself. literally nulled allocated storage to stick stuff into
  } BasicHashTable;
  */
  return ht;
}

/****
  Fill this in.

  If you are overwriting a value with a different key, print a warning.
  isn't this collision protection? I mean not really protection but notification
  We're not doing collision protection today. If a Pair already exists at a 
  given index, print a warning, but otherwise just overwrite it.


  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_insert(BasicHashTable *ht, char *key, char *value)
{
  //both keys are going to get hashed out to the same thing I gotta figure out how to grab t
  // if (key != ht[hash(ht[key], capacity)])// ok the syntax is getting creayay cray
  // tell me in english what's the condition you're checking
  //NOT DOING ALL OF THIS YET! I'm trying to see if the key we are passing the insert is the same as the key in the basicHashtable @ that index so I was trying to 
  //hash the new key because it's the same number which would give you the same index so you would know which key to compare it with.
  // Gimme a minute. You can come into the zoom if I take forever
    int index = hash(key, ht->capacity);  // before you continue on this, make sure BasicHashTable is done
    // R: what's the ht[] around the hash function call doing? the not anything I want it to be doing
    // R: Ok sweet. hash returns an int, which you're saving into an int variable "index"
    if (ht->storage[index] != NULL) // <- where's your array in the `ht` structure? Yessir this is looking less foreign 
    {
      printf("That spot is already taken!");
       ht->storage[index] = create_pair(key, value);
    } 
    else 
    {
      // does this make sense when ht->storage[index] is NULL or when it is not NULL?  oh no  lol what? I need to swap because if it's not null then it's taken then i'm creating the pair for the spot
      // and if it is null then we just do it. In either case, you're going to stick it in there. The only thing is you print a warning if you overwrite. This looks good I think k onto the next one
      ht->storage[index] = create_pair(key, value);
    }
    // the only thing now is that the spec allows you to overwrite an existing value in the index, but you have to print a little warning
    // before you do
    // so you just have to rearrange this logic around and put a print statement before you overwrite if something already exists in that index
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_remove(BasicHashTable *ht, char *key)
{
 int index = hash(key, ht->capacity);

 if (ht->storage[index] != NULL) {
   free(ht->storage[index]);
   ht->storage[index] = NULL;
 }
 else 
 {
   printf("That pair does not exist.");
 }
}

/****
  Fill this in.

  Should return NULL if the key is not found.
 ****/
char *hash_table_retrieve(BasicHashTable *ht, char *key) // you're given the string key to look for already up here
{
  int index = hash(key, ht->capacity);

  // a hash converts a string key to a number. you should therefore be passing a string 
  //okay yeah because the key is getting passed in and that's all we have to search by is the hash of they key. I started to over think it. :+1:
  // is it ht[key] or ht->storage[key]? I know that's an array of pointers though so maybe it won't expect an int ht[key] is
  if (ht->storage[index] != NULL)  
    {
      printf("this is a label: %s\n" ,ht->storage[index]->value);
      return ht->storage[index]->value; // it's not that easy is it? because I mean value is in pair in storage but idk if these arrows work like this. 
      //but then again storage is a array of pointers of pairs so maybe it's just storage[index]-> value
      // if ht->storage[index] isn't NULL, it should already be a pair struct, right? because that's what you insert at each index
      // e.g. you don't need ->pair. ht->storage[index] is already a pair but would ht->storage[index]->value grab what I want? which is the value 
    } 
  else 
  {
    return NULL;
  }
    
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(BasicHashTable *ht)
{
  // if (ht->storage[index] != NULL) {
  //  for(int i = 0, i < ht->capacity, i++) {
  //  free(ht->storage);
     
  //  }
if (ht != NULL) {
  free(ht->storage);
  free(ht);
} 
}


#ifndef TESTING
int main(void)
{
  struct BasicHashTable *ht = create_hash_table(16);
  printf("Hash table created\n");
  hash_table_insert(ht, "line", "Here today...\n");
  printf("Insertion successful\n");
  printf("%s", hash_table_retrieve(ht, "line"));
  printf("Retrieval successful\n");
  hash_table_remove(ht, "line");
  printf("Removal successful");
  if (hash_table_retrieve(ht, "line") == NULL) {
    printf("...gone tomorrow. (success)\n");
  } else {
    fprintf(stderr, "ERROR: STILL HERE\n");
  }

  destroy_hash_table(ht);

  return 0;
}
#endif
