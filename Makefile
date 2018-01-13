CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Werror -g 
CFLAGS2 = -Wall -Wextra -Wpedantic -Werror -g 
CLAGS3 =  -Wpedantic -g
OBJECTS = bv.o bf.o aes.o ll.o hash.o lex.yy.o  banhammer.o

banhammer : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o banhammer -lfl
bv.o : bv.c
	$(CC) $(CFLAGS) -c bv.c

aes.o : aes.c
	$(CC) $(CFLAGS) -c aes.c

bf.o : bf.c
	$(CC) $(CFLAGS) -c bf.c

ll.o : ll.c
	$(CC) $(CFLAGS2) -c ll.c

hash.o : hash.c
	$(CC) $(CFLAGS2) -c aes.c ll.c hash.c

lex.yy.c : words.l
	flex words.l

lex.yy.o : lex.yy.c
	$(CC) $(CFLAGS3) -c lex.yy.c

banhammer.o : banhammer.c
	$(CC) $(CFLAGS)  -c banhammer.c 

.PHONY: clean
clean:
	rm $(OBJECTS) banhammer

.PHONY: all
all : banhammer
banhammer : $(OBJECTS)
	$(CC) $(CFLAGS3) $(OBJECTS) -o banhammer -lfl
