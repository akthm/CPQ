//
// Created by akthm on 18/03/2024.
//

#ifndef STRINGHASH_LABELMAPPER_H
#define STRINGHASH_LABELMAPPER_H
#include "stringHasher.h"
#include <ctype.h>
#define MAP_SIZE 10 //map table size
#define MAX_LABEL_SIZE 20



Node *initLabelMapper();
int addLabel(Node *root, const char *label, int line);
int getLabelLine(Node *root, const char *label);

#endif //STRINGHASH_LABELMAPPER_H
