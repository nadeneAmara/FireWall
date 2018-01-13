# include "ll.h"
# include <stdio.h>
# include <stdint.h>
# include <stdlib.h>
# include <string.h>

extern bool moveToFront;
extern uint32_t distance;
extern uint32_t count;

listNode *newNode (const char *old, const char *new)
{
	listNode *l = (listNode *) malloc(sizeof(listNode));
	if (l)
	{
		l->oldspeak = strdup(old);
		l->newspeak = strdup(new);
		l->next = NIL;
		return l;
	}
	else
	{
		return NIL;
	}
}

void delNode (listNode *l)
{
	free(l->oldspeak);
	free(l->newspeak);
	free(l);
	return;
} 

void delLL (listNode *l)
{
	while (l != NIL)
	{
		listNode *temp = l;
		l = l->next;
		delNode(temp);
	}
	return;
}

/*Searches for word in linked list. If moveToFront rule is enabled, a word will
 * be moved to the front of the list when found. This will optimize search time.
*/
listNode *findLL (listNode **l, const char *word)
{
	count = count + 1;
	listNode *prev = NIL;
	for (listNode *t = *l; t != NIL; t = t->next)
	{
		distance = distance + 1;
		if (strcmp(t->oldspeak, word) == 0)
		{
			if (moveToFront && prev != NIL)
			{
				prev->next = t->next;
				t->next = *l;
				*l = t;
			}
			return t;
		}
		prev = t;
	} 
	return NIL;
}

//If already in list, return it. If not,insert at front
listNode *insertLL(listNode **l, const char *old, const char *new)
{
        if (findLL(l,old))
	{
        	return *l;
	}
	else
	{
		listNode *temp = newNode(old,new);
		temp->next = *l;
		return temp;
	}
}



void printLL(listNode *l)
{
	for (listNode *t = l; t != NIL; t = t->next)
	{
		if (t->newspeak != NULL)
		{
			printf("%s -> %s",t->oldspeak,t->newspeak);
			printf("\n");
		}
		else
		{
			printf("%s",t->oldspeak);
			printf("\n");
		}
	}
	printf("\n");
	return;
}
	
