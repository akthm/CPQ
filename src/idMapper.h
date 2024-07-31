//
// Created by akthm on 18/03/2024.
//

#ifndef STRINGHASH_IDMAPPER_H
#define STRINGHASH_IDMAPPER_H
#include "stringHasher.h"
#include <ctype.h>
#define MAP_SIZE_ID 62 //map table size


Node *initIdMapper();
int addMapId(Node *root, const char *word, char* tmp);
char *getTmpId(Node *root, const char *word);
int idMExists(Node *root, const char *word);


#endif //STRINGHASH_LABELMAPPER_H
