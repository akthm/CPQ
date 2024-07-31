//
// Created by akthm on 18/03/2024.
//
#include "synMapper.h"



void *SYNAllocator(void *elem){
    SYNode *node = malloc(sizeof (SYNode));
    if(!node){
        return NULL;
    }
    SYNode *sn = (SYNode*)elem;
    node->type = sn->type;
    node->isInit = sn ->isInit;
    return node;
}

void SYNFree(void *elem){
    if (elem){
        free(elem);
    }
}

Node *initSYNMapper(){
    Node *node = initNode(NUM_OF_VALUES, &alphaNumHasher, &SYNFree, &SYNAllocator);
    if(!node){
        return NULL;
    }
    return node;
}

int addID(Node *root, const char *name, Type type){
    if(root && name){
        if (idExists(root, name)){
            return -1; //double decleration
        }
        SYNode tmp;
        tmp.type = type;
        tmp.isInit = 0;
        // printf("%s %s\n", name, type == N ? "int" : "float");
        return addElem(root, name,&tmp);
    }
    return 0;
}

int idExists(Node *root, const char *name){
    if(root && name){
        return getElem(root, name) != NULL;
    }
    return 0;
}

Type getIdType(Node *root, const char *name){ //no except should always be checked before
    return ((SYNode *)getElem(root, name))->type;
}

int idIsInit(Node *root, const char *name)
{
    void *elem;
    if(elem = getElem(root, name)){
        SYNode *sn = (SYNode *) elem;
        return sn->isInit;
    }
    return 0;
}

void idSetInit(Node *root, const char *name)
{   void * elem;
    if(elem = getElem(root, name)){
        SYNode *sn = (SYNode *)elem;
        sn->isInit = 1;
    }
}
