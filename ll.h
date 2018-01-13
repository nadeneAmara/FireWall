# ifndef NIL
# define NIL (void *) 0
# endif
# ifndef MTF
# define MTF true
# endif

# ifndef _LL_H
# define _LL_H

# include <stdbool.h>
# include <stdint.h>

extern bool moveToFront;

typedef struct listNode listNode;

struct listNode
{
	char *oldspeak, *newspeak;
	listNode *next;
};

listNode *newNode(const char *, const char *); //allocates memory for and initializes new node

void delNode (listNode *);

void delLL (listNode *);

listNode *insertLL(listNode **, const char *, const char *); //inserts new node at front of list

listNode *findLL(listNode **, const char*); //finds requested node

void printLL(listNode *);
# endif
