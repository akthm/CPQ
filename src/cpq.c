#include "cpq.tab.h"


#define TMP_FILE_NAME "tmp-file"

int nameCheck(char *fileName, char *target){
    int len = strlen(fileName);
    bool a = fileName[len - 1] == 'u' ||fileName[len - 1] == 'U';
    bool b = fileName[len - 2] == 'o' ||fileName[len - 2] == 'O';
    bool c = fileName[len - 3] == '.';
    if(!a || !b || !c ){
        fprintf(stderr, "File is Not of type .ou\n");
        return false;
    }
    int i ;
    for (i=0; i < len - 2 ; ++i){
        target[i] = fileName[i];
    }
    target[i] = 'q';
    target[i+1] = 'u';
    target[i+2] = 'd';
    target[i+3] = 0;
    return 1;
}


int main(int argc, char* argv[]){
    extern Node *root; // global variables search tree (name - type - is initialized)
    extern NodeAdder * adder; // global linked list used in adding variables to tree    extern FILE *yyin; /* defined by flex */
    extern FILE *yyout; /* defined by flex */
    extern FILE *yyin;
    extern int ABORT; //global abort flag declared in cpq.y
    FILE *outFile;
    char filename[MAX_ID_SIZE];
    if (argc > 1) {
        if(!nameCheck(argv[1], filename)){
            return 1;
        }
        yyin = fopen(argv[1], "r" );
        if (yyin == NULL ){fprintf(stderr, " input file not found!\n"); return 1;}

        yyout = fopen(TMP_FILE_NAME, "w+");
        if (!yyout ) {
            fprintf(stderr, "file not found!\n");
            fclose(yyin);
            return 1;
        }

    }
    else{
        fprintf(stderr, "USAGE: cpq <source>\n");  
        return 1;} // no source file
    root = initSYNMapper();
    if(!root){
        fprintf(stderr, "SYN Map Allocation failed!\n");
    }
    adder = initAdder();
    if(!adder){
        freeTree(root);
        fprintf(stderr, "Adder Allocation failed\n");
    }
    int res = yyparse();
    freeTree(root);
    freeAdder(adder);
    if (!ABORT){
        outFile = fopen(filename, "w+");
        if (!outFile) {
            fprintf(stderr, "output file not found!\n");
            fclose(yyout);
            fclose(yyin);
            return 1;
        }
        replace_labels(outFile, yyout);
        fclose(outFile);}

    fclose(yyout);
    fclose(yyin);
    if(remove(TMP_FILE_NAME)){
        fprintf(stderr, "temp file removal failed!\n");

        return 0 ; }
}
