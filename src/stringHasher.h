#ifndef STRING_HASHER_H
#define STRING_HASHER_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NUM_OF_VALUES 62 // in our case
typedef int(*Hasher)(unsigned int);
typedef void(*Freeer)(void *);
typedef void *(*Allocer)(void *);

typedef struct 
{   
    bool isReached;
    bool isFull;
    void *value; 
    void **subs;
    size_t size;
    Hasher hashFunc;
    Freeer freeFunc;
    Allocer allocFunc; // copy alloc func
    
} Node;



int alphaNumHasher(unsigned int c);
Node *initNode(size_t size, Hasher hashFunc, Freeer freeFunc, Allocer allocFunc);
int addElem(Node *node,const char *word, void *elem);
void *getElem(Node *root,const char *word);
void freeTree(Node *root);

#endif // STRING_HASHER_H
