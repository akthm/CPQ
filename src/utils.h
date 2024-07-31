#ifndef UTILS_H
#define UTILS_H


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include "synMapper.h"
#include "labelMapper.h"
#include "idMapper.h"

extern int ABORT;
#define OUTFILE yyout
extern FILE *OUTFILE;

#define TMP_SIZE 30
#define MAX_ID_SIZE 31
#define MAX_LABELS 100
#define MAX_LINE_LENGTH 100
#define MAX_NUMBER_IDS 100

/*
 * N -- int
 * FL -- float
 */
//typedef enum { N, FL } Type; // implemented in synMapper



typedef struct {
    char* text;
    void *next;
} CharNode;

typedef struct {
    CharNode *root;
    CharNode *last;
} NodeAdder;

typedef struct {
    int ival;
    float fval;
    Type type;
} NumVal;

typedef struct {
    char result[MAX_ID_SIZE];
    Type type;
} ExpType;



/* functions borrowed from homework questions */
void gen(int count, ...); //variable arg printer (output file)
void err(const char *msg); // for allocation errors
void newtemp(void *in);
void newlabel(void *in);


/* CharNode actions */
NodeAdder *initAdder();
void addToAdder(NodeAdder *adder, const char *text);
CharNode *initCharNode( const char *text, CharNode *next);
void loadNodesInto(Node *root, NodeAdder *adder, Type type);
void freeAdder(NodeAdder *adder);


void replace_labels(FILE *outputFile, FILE *tmpFile); // final screening 

#endif // utils.h