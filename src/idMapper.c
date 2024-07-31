#include "idMapper.h"




void *strAllocer(void *elem){
    char *other = *((char **) elem);
    char *res = malloc(strlen(other) + 1);
    if(!res){
        return NULL;
    }
    strcpy(res, other);
    return res;
}

void strFree(void *elem){
    if(elem){
        free(elem);
    }
}


Node *initIdMapper(){
    Node *res = initNode(MAP_SIZE_ID, &alphaNumHasher, &strFree, &strAllocer);
    if(!res){
        return NULL;
    }
    return res;
}


int addMapId(Node *root, const char *word, char* tmp){
    if(word[strlen(word)-1] == ':'){
        return -1;
    }
    if(!root || !word || !tmp){
        fprintf(stderr, "Null pointer at addMapId\n");
        return -1;
    }
    return addElem(root, word, &tmp);
}

bool legalId(const char* word){
    if (!word){
        return false;
    }
    for(int i=0; i< strlen(word); ++i){
        if(!isalnum(word[i])){
            return false;
        }
    }
    return true;
}

char *getTmpId(Node *root, const char *word){
    if(!legalId(word)){
        return NULL;
    }
    if(root && word){
        void *res = getElem(root, word);
        if (!res ){return NULL;}
        return (char *)res;
    }
    return NULL;
}



int idMExists(Node *root, const char *word){
    if(!legalId(word)){
        return 0;
    }
    return getElem(root, word) != NULL;
}