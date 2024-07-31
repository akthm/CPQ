//
// Created by akthm on 18/03/2024.
//

#ifndef STRINGHASH_SYNMAPPER_H
#define STRINGHASH_SYNMAPPER_H

#include "stringHasher.h"
typedef enum { N, FL } Type;


typedef struct {
    Type type;
    int isInit;
} SYNode;

Node *initSYNMapper();
int addID(Node *root, const char *name, Type type);
int idExists(Node *root, const char *name);
Type getIdType(Node *root, const char *name);
int idIsInit(Node *root, const char *name);
void idSetInit(Node *root, const char *name);



#endif //STRINGHASH_SYNMAPPER_H
