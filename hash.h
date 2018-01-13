# ifndef NIL
# define NIL (void *) 0
# endif

# ifndef _HASH_H
# define _HASH_H
# include <stdint.h>
# include "ll.h"

typedef struct hashTable hashTable;
extern uint32_t calls;
struct hashTable
{
	uint32_t s[4]; //Salt
	uint32_t l; //length
	listNode **h; //Array of pointers to linked lists
};

hashTable *newHT(uint32_t, uint32_t []);

void delHT(hashTable *);

listNode *findHT(hashTable *, const char *); //makes call to respective linkedlist

void insertHT(hashTable *, const char *, const char*); //inserts to front of linked list

uint32_t hash(hashTable *, const char *);

void printHT(const hashTable *);

#endif
