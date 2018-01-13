# include "hash.h"
# include "ll.h"
# include "bf.h"
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <errno.h>
# include <ctype.h>

# define EMPTY	""

# define MAXWORD       100
# define HASHSZ	     10000
# define BF	 (1 << 20)

# define VERBOSE(x)	{ if (verbose) printf("%s", x); }

# ifndef PREFIX
# define PREFIX "/afs/cats.ucsc.edu/users/g/darrell/"
#endif

# define BADSPEAK PREFIX "/badspeak.txt"
# define NEWSPEAK PREFIX "/newspeak.txt"
# define ESPERANTO PREFIX "/Esperanto.txt"

int yylex(void);
extern char* yytext;
extern FILE* yyin;

bool moveToFront = MTF;

uint32_t distance = 0;
uint32_t count = 0;
uint32_t words = 0;
uint32_t trans = 0;
uint32_t text = 0;

static inline void makeLowercase(char *word)
{
	for (int i = 0; word[i] != '\0'; i += 1) { word[i] = tolower(word[i]); }
}

/* Loads forbidden words and sets them in bloom filters.
 * The bloom filter allows the program to quickly check
 * to see if the word is in the dictionary. An empty 
 * translation is then inserted into the hash table.
 * 
 */ 
void loadDictionary(hashTable *h, bloomF *a, bloomF *b, char *dictionary)
{
	char word[MAXWORD];
	FILE *f = fopen(dictionary, "r");
	if (f == NULL)
	{
		perror("loadDictionary");
		exit(errno);
	}
	while (fscanf(f, "%s", word) != EOF)
	{
		makeLowercase(word);
		words += 1;

		setBF(a, word); setBF(b, word); // Load Bloom filters
		insertHT(h, word, EMPTY); // Add empty translation to table
	}
	fclose(f);
	return;
}

/* Loads replacements for forbidden words. Old
 * version of word is set in the bloom filter.
 * Translation is then inserted into the hashtable
 * for the corresponding word.
 */
void loadTranslations(hashTable *h, bloomF *a, bloomF *b, char *replacements)
{
	char word[MAXWORD], tran[MAXWORD];
	FILE *f = fopen(replacements, "r");
	if (f == NULL)
	{
		perror("loadTranslations");
		exit(errno);
	}
	while (fscanf(f, "%s %s", word, tran) != EOF)
	{
		makeLowercase(word);
		makeLowercase(tran);
		trans += 1;

		setBF(a, word); setBF(b, word); // Add to Bloom filter

		listNode *t = findHT(h, word);

		if (t)
		{
			free(t->newspeak);
			t->newspeak = strdup(tran);	// Replace if translation already exists
		}
		else { insertHT(h, word, tran); }	// New translation
	}
	fclose(f);
	return;
}


int main (int argc, char *argv[])
{
	bool stats      = false; // Print statistics?
	bool verbose    = false; // Be verbose?
	bool joyCamp    = false; // Reeducation?
	bool esperanto  = false; // Add Esperanto?
	bool mustRevise = false; // Require revision?

	uint32_t hashSz = HASHSZ; // Default hash table size
	uint32_t BFSz   = BF;	  // Default Bloom filter size

	//Salts for bloom filters and hashtables
	uint32_t initA[] = {0xDeadD00d, 0xFadedBee, 0xBadAb0de, 0xC0c0aB0a}; // First Bloom filter 
	uint32_t initB[] = {0xDeadBeef, 0xFadedB0a, 0xCafeD00d, 0xC0c0aB0a}; // Second Bloom filter 
	uint32_t initH[] = {0xDeadD00d, 0xFadedBee, 0xBadAb0de, 0xC0c0Babe}; // Hash table
	
	bloomF *a = newBF(BFSz,initA);
        bloomF *b = newBF(BFSz,initB);
	hashTable *h = newHT(hashSz,initH);
        
	int option;
	while ((option = getopt(argc, argv, "-embsh:f:")) != -1)
	{
		switch (option)
		{
			case 'e':
			{
				esperanto = true;
				break;
			}
			case 'h':
			{
				hashSz = atoi(optarg);
				break;
			}
			case 'f':
			{
				BFSz = atoi(optarg);
				break;
			}
			case 'm':
			{
				moveToFront = true;
				break;
			}
			case 'b':
			{
				moveToFront = false;
				break;
			}
			case 's':
			{
				stats = true;
			}
		}
	}

	VERBOSE("Loading dictionary...");
	loadDictionary(h, a, b, BADSPEAK);
	VERBOSE("done.\n");

	VERBOSE("Loading translations...");
	loadTranslations(h, a, b, NEWSPEAK);
	VERBOSE("done.\n");

	if (esperanto)
	{
		VERBOSE("Loading Esperanto...");
		loadTranslations(h, a, b, ESPERANTO);
		VERBOSE("done.\n");
	}

	listNode *revList = NIL; // Words that must be revised
	listNode *badWord = NIL; // Words that are forbidden

	/* Uses flex lexical anaylyzer to handle all user input.
	 * Check for membership in both bloom filters. If the 
	 * word is set in both filters, check for translation.
	 * If no translation, joyCamp. Otherwise, provide 
	 * replacement words.
	 */
	while (yylex() != -1)
	{
		char s[MAXWORD];

		strncpy(s, yytext, MAXWORD);

		for (int i = 0; s[i] != '\0'; i += 1) { s[i] = tolower(s[i]); }

		text += 1; // Count words in the text

		if (memBF(a, s) && memBF(b, s)) // Check for membership in both bloom filters
		{
			listNode *t = findHT(h, s); // Check for translation
			if (t)
			{
				if (strlen(t->newspeak) == 0)	//If no translation, joycamp
				{
					badWord = insertLL(&badWord, s, EMPTY); joyCamp = true;
				}
				else
				{
					 revList = insertLL(&revList, t->oldspeak, t->newspeak); mustRevise = true;
				}
			}
		}
	}

	if (!stats && !joyCamp && mustRevise)
	{
		printf("Dear Comrade,\n\n");
		printf("Submitting your text helps to preserve feelings and prevent\n");
		printf("badthink. Some of the words that you used are not goodspeak.\n");
		printf("The list shows how to turn the oldspeak words into newspeak.\n\n");
		printLL(revList);
	}

	if (!stats && joyCamp)
	{
		printf("Dear Comrade,\n\n");
		printf("You have chosen to use degenerate words that may cause hurt\n");
		printf("feelings or cause your comrades to think unpleasant thoughts.\n");
		printf("This is doubleplus bad. To correct your wrongthink and\n");
		printf("save community consensus we will be sending you to joycamp\n");
		printf("administered by Miniluv.\n");
		printf("\nYour errors:\n\n");
		printLL(badWord);
		if (revList)
		{
			printf("\nThink on these words during your vacation!\n\n");
			printLL(revList);
		}
	}

	if (stats && count > 0)
	{
		printf("Seeks %u, Average %lf, ", distance, (double) distance / (double) count);
		printf("Dictionary %u, Translations %u, Text %u, ", words, trans, text);
		printf("Densities: %lf, %lf\n", countBF(a)/(double) lenBF(a), countBF(b)/(double) lenBF(b));
	}

	delBF(a); delBF(b); delHT(h); delLL(revList); delLL(badWord);
}
