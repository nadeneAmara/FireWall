# include <stdlib.h>
# include <stdint.h>
# include <string.h>
# include <stdio.h>
# include "aes.h"
# include "hash.h"

extern uint32_t count;

//Creates new hashtable with l length and specified salt
hashTable *newHT (uint32_t l, uint32_t b[])
{
	hashTable *ht = (hashTable *) malloc(sizeof(hashTable));
	if (ht)
	{
		ht->h = malloc(l*sizeof(listNode *));
		ht->s[0] = b[0];
		ht->s[1] = b[1];
		ht->s[2] = b[2];
		ht->s[3] = b[3];
        	ht->l = l;
        	return ht;
	}
	else
	{
		return NIL;
	}

}

//Delete hash table
void delHT (hashTable *ht)
{
	for (uint32_t i = 0; i < ht->l; i++)
        {
		delLL(ht->h[i]);
        }

	free(ht->h);
	free(ht);
	return;
}

//Look for existing translation entry
listNode *findHT (hashTable *ht, const char *old)
{
	uint32_t index = hash(ht,old) % ht->l;
	return findLL(&ht->h[index],old);
} 

//insert new entry into hashtable
void insertHT (hashTable *ht, const char *old, const char *new)
{
	uint32_t index = hash(ht,old) % ht->l;
	ht->h[index] = insertLL(&ht->h[index],old,new);
	return;
}

void printHT (const hashTable *ht)
{
	for (uint32_t i = 0; i < ht->l; i++)
	{
		if (ht->h[i] != NIL)
		{
			printLL(ht->h[i]);
			printf("\n");
		}
	}
	return;
}

static inline int realLength(int l)
{
	return 16 * (l / 16 + (l % 16 ? 1 : 0));	
}

uint32_t hash (hashTable *h, const char *key)
{
	uint32_t output[4] = {0x0};
	uint32_t sum = 0x0;
	int keyL = strlen(key);
	uint8_t *realKey = (uint8_t*) calloc(realLength(keyL), sizeof(uint8_t));

	memcpy(realKey, key, keyL);
	for (int i = 0; i < realLength(keyL); i += 16)
	{
		AES128_ECB_encrypt((uint8_t *) h->s, (uint8_t *) realKey + i, (uint8_t *) output);
		sum ^= output[0] ^ output [1] ^ output[2] ^ output[3];
	}
	free(realKey);
	return sum;
}
