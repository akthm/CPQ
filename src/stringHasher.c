
#include "stringHasher.h"

/*
a - 97  -- 65 CAP
b -
c - 
d - 
e - 
f -
g -
h - 
i - 
j - 
k - 
l - 
m - 
n - 
o - 
p - 
q - 
r - 
s -
t -
u - 
v - 
w - 
x -
y -
z - 122 -- 90 CAP
0 - 48
1 - 
2 -
3 - 
4 -
5 -
6 - 
7 -
8 -
9 - 57

*/

// 0-9 => 0 - 9 index
// A-Z => 10 - 35 index
// a-z => 36 - 61

int alphaNumHasher(unsigned int c){ //
    if(c < 58){ // number
        return c - 48;
    }
    else if (c > 64 && c < 91){ // capital
        return c - 55;
    }
    else {
        return c - 61;
    }
};


Node *initNode(size_t size, Hasher hashFunc, Freeer freeFunc, Allocer allocFunc){
    Node *tmp = malloc(sizeof(Node));
    if (!tmp){
        return NULL;
    }
    tmp->isReached = false; // subs inited
    tmp->isFull = false; // value full
    tmp->hashFunc = hashFunc;
    tmp->freeFunc = freeFunc;
    tmp->allocFunc = allocFunc;
    tmp->subs = NULL;
    tmp->size = size;
    return tmp;
}

int initSubs(Node *node){
    bool panic = false;
    node->subs = malloc(sizeof(Node *) * node->size);
    Node **subs = (Node **)node->subs;
    if (!subs){
        return 0;
    }
    int i;
    for (i=0; i < node->size; ++i){
        subs[i] = initNode(node->size, node->hashFunc, node->freeFunc, node->allocFunc);
        if(!subs[i]){
            --i;
            panic = true;
            break;
        }
    }
    if (panic){
        for (int j=0; j < i; ++j){
            subs[j]->freeFunc(&subs[j]->value);
            free(subs[j]);
        }
        free(subs);
        return 0;
    }
    node->isReached = true;
    return 1;
}

int addChar(Node *node, unsigned int c ){
    if(!node->isReached){
        if(!initSubs(node)){
            return 0;
        }
    }
    int res = node->hashFunc(c);
    Node **subs = (Node **)node->subs;
    return initSubs(subs[res]);
}


int addElem(Node *node,const char *word, void *elem){ // 0 alloc error -- -1 word error;
    Node *runner = node;
    if(!word || !node){
        fprintf(stderr, "Null pointer in addElem!\n");
        return -1; //not alloc error
    }
    int i;
    for (i=0; i < strlen(word); ++i){
        int res = runner->hashFunc(word[i]);
        if(!runner->isReached){
            if(!addChar(runner, word[i])){
                return 0; // alloc error
            }
        }
        Node **nArr = (Node **)runner->subs;
        runner = nArr[res];
    }
    runner->value = node->allocFunc(elem);
    if (!runner->value){
        return 0; // alloc error 
    }
    runner->isFull = true;
    return 1;
}

void *getElem(Node *root,const char *word){
    if(!word || !root || !root->isReached){
        return NULL;
    }
    Node *runner = root;
    for (int i=0; i<strlen(word); ++i){
        if(!runner->isReached){
            return NULL;
        }
        int res = runner->hashFunc(word[i]);
        Node **nArr = (Node **)runner->subs;
        runner = nArr[res];
    }
    if(!runner->isFull){
        return NULL;
    }
    return runner->value;
}

void freeNode(Node *nd){
    if(nd){
        Node *tmp = nd;
        if(tmp->isFull){
            tmp->freeFunc(tmp->value);}
        free(tmp);
        nd = NULL;
    }
}

void freeTree(Node *root){
    if(root){
        if(root->isReached){
            for (int i=0; i<root->size; ++i){
                freeTree(((Node **)root->subs)[i]);
            }
            free(root->subs);
        }
        freeNode(root);
        root = NULL;
    }
}






