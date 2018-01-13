# ifndef NIL 
# define NIL (void *) 0
# endif
# ifndef _BF_H
# define _BF_H
# include <stdint.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>

typedef struct bloomF 
{
	uint8_t *v; //vector
	uint32_t l; //length
	uint32_t s[4]; //salt
} bloomF;

//Each function has its own hash function

uint32_t hashBF(bloomF *, char *);

//Create a new Bloom filter of a given length and hash function

static inline bloomF *newBF(uint32_t l, uint32_t b[])
{
	bloomF *bf = calloc(l,sizeof(uint32_t));
	bf->v = calloc(l , sizeof(uint8_t));
	memcpy(bf->s,b,4);
	bf->l = l;
	return bf;
}

//Delete a Bloom filter

static inline void delBF(bloomF *v)
{
	//free(v->s);
	free(v->v);
	free(v);
}

//Return the value of position k in the Bloom filter

static inline uint32_t valBF(bloomF *x, uint32_t k)
{
	return ((x -> v)[k >> 3] & (01 << (k & 07))) >> (k & 07);
}

static inline uint32_t lenBF(bloomF *x) 
{
	return x->l;
}

//Count bits in the Bloom filter

static inline uint32_t countBF(bloomF *b)
{
	uint32_t count = 0;
	for (uint32_t i = 0; i < (b -> l); i++)
	{
		if (valBF(b,i))
		{
			count++;
		}
	}
	return count;
}

//Set an entry in the Bloom filter

static inline void setBF(bloomF *x, char * key)
{
	uint32_t index = (hashBF(x,key) % x->l);
	(x -> v)[index >> 3] |= (01 << (index & 07));
}

//Clear an entry in the Bloom filter

static inline void clrBF(bloomF *x, char *key)
{
	uint32_t index = (hashBF(x,key) % x->l);
	(x -> v)[index >> 3] &= ~(01 << (index & 07));
}

//Check membership in the Bloom filter

static inline uint32_t memBF(bloomF *x, char *key)
{
	uint32_t index = (hashBF(x,key) % x->l);
	return valBF(x,index);
}

static inline void printBF(bloomF *x)
{
	for (uint32_t i = 0; i < x -> l; i++)
	{
		if (valBF(x,i))
		{
			printf("1");
		}
		else
		{
			printf("0");
		}
	}
	printf("\n");
}

# endif
