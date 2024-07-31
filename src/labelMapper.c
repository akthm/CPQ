//
// Created by akthm on 18/03/2024.
//
#include "labelMapper.h"

void *intAllocer(void * elem){
    int el = *((int * )elem);
    int *res = malloc(sizeof(int));
    if(!res){
        return NULL;
    }
    *res = el;
    return res;
}

void intFree(void * elem){
    if (elem){
        free(elem);
    }
}

int labelHasher(unsigned int c){ //only numbers
    return c - 48;
}


Node *initLabelMapper(){
    Node *res = initNode(MAP_SIZE, &labelHasher, &intFree, &intAllocer);
    if(!res){
        return NULL;
    }
    return res;
}

int extractLblNumber(const char *lbl, char *target){
    int len = strlen(lbl);
    if(lbl[0] != 'L' || lbl[len - 1] != ':'){
        return 0;
    }
    for(int i=1; i< len - 1; ++i){
        if(!isdigit(lbl[i])){
            return 0;
        }
        target[i-1] = lbl[i];
    }
    target[len- 2 ] = 0; //null terminator
    return 1;
}


int addLabel(Node *root, const char *label, int line){
    if(!root || !label){
        return -1;
    }
    char lnum[MAX_LABEL_SIZE];
    if(!extractLblNumber(label, lnum)){
        fprintf(stderr, "%s is NOT OF LABEL TYPE (L<num>:)\n", label);
        return -1;
    }
    return addElem(root, lnum, &line);
}

int getLabelLine(Node *root, const char *label){
    if(!root || !label){
        return -1;
    }
    void *elem;
    char tmp[MAX_LABEL_SIZE];
    if(!extractLblNumber(label, tmp)){
        return -1;
    }
    if((elem = getElem(root, tmp))){
        return *((int *)elem);
    }
    return -1;
}

