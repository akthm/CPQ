 #include "utils.h"


int tmp_counter = 0; /* Global temp counter */
int lbl_counter = 0;  /* Global label counter */

void gen(int count, ...){ //simple VA input printer
        extern FILE *OUTFILE;
        if(ABORT)
            return;
        va_list args;
        va_start(args, count);
        for (int i=0; i<count; ++i){
            char *tmp = va_arg(args, char*);
            fprintf(OUTFILE, "%s ", tmp);
        }
        fprintf(OUTFILE, "\n");
        va_end(args);
 }


void err(const char* msg){
    extern NodeAdder *adder; // main adder
    extern Node *root; // id mapper
    fprintf(stderr, "%s\n", msg);
    freeAdder(adder);
    freeTree(root);
    exit(EXIT_FAILURE);
}



/* input local string address to copy string into */
void newtemp(void* in){ 
    char res[TMP_SIZE] = "tmp";
    char num[TMP_SIZE - 3];
    sprintf(num, "%d", tmp_counter);
    tmp_counter ++ ;
    strcat(res, num);
    memcpy(in, res, strlen(res) + 1); }

/* input local string address to copy string into */
void newlabel(void* in){ 
    char res[TMP_SIZE] = "L";
    char num[TMP_SIZE - 1];
    sprintf(num, "%d", lbl_counter);
    lbl_counter ++ ;
    strcat(res, num);
    strcat(res, ":");
    memcpy(in, res, strlen(res) + 1); }
    



CharNode *initCharNode( const char *text, CharNode *next) { 

    CharNode *res = malloc(sizeof(CharNode));
    char *tmptext = malloc(strlen(text) + 1);
    if(!res || !tmptext){
        return NULL;
    }
    strcpy(tmptext, text);
    res->text = tmptext;
    res->next = next;
    return res;
}

void loadNodesInto(Node *root, NodeAdder *adder, Type type){
    extern int ABORT;
    extern int yylineno;
    CharNode *temp = adder->root;
    while (temp){
        int fl;
        if(!(fl = addID(root, temp->text, type))){
            err("allocation error at load nodes"); //terminal exit 
        }
        if (fl == -1){
            fprintf(stderr, "Double declaration of ID : %s on line: %d\n", temp->text, yylineno);;
            ABORT = 1;
        }
        CharNode *otmp = (CharNode *)temp->next;
        free(temp->text);
        free(temp);
        temp = otmp;

    }
    adder->root = NULL;
    adder->last = NULL;
}

void freeAdder(NodeAdder *adder)
{
        if(adder){
            NodeAdder *temp = adder;
            if (temp->root){
                CharNode *tn = temp->root;
                while (tn)
                {
                    CharNode *next = (CharNode *)tn->next;
                    free(tn->text);
                    free(tn);
                    tn = next;
                }
            }
            free(adder);
        }
}

NodeAdder *initAdder()
{
    NodeAdder *res = malloc (sizeof(NodeAdder));
    if(!res){
        return NULL;
    }
    return res;
}

void addToAdder(NodeAdder *adder, const char *text)
{
    if(!adder || !text){
        fprintf(stderr, "NULL Value sent to adder terminate!\n");
        extern int ABORT;
        ABORT = 1;
        return;
    }
    CharNode *node = initCharNode(text, NULL);
    if(!node){
        err("Adder allocation Error!\n");
    }

    if (adder->root)
    {
        adder->last->next = node;
        adder->last = node;
    }
    else
    {
        adder->root = node;
        adder->last = node;
    }
}


/* complex function to replace labels with lines and "tempifying" uppercase variables */
void replace_labels(FILE *outputFile, FILE *tmpFile) { // final screening
    FILE  *outFile = outputFile; // no opening happening here for ease
    fseek(tmpFile, 0, SEEK_SET); // reset file pointer
    Node *lblMap = initLabelMapper();
    if (!lblMap){
        fprintf(stderr, "Alloc Error at lable Map in final screening\n"); //will be freed at the end
        return;
    }
    Node *idMap = initIdMapper();
    if(!idMap){
        fprintf(stderr, "Alloc Error IDMMapper in final screening\n");
        freeTree(lblMap);
        return;
    }
    // IdMap idMapper;
    // idMapper.count = 0;
    int labelCount = 0;
    int lineNo = 1;
    char line[MAX_LINE_LENGTH];
    char *token;
    int prev = 0; // previous flag to detect multiple lines of labels
    int prevLine;
    while(fgets(line, MAX_LINE_LENGTH, tmpFile)){ // label and uppercase id detection loop
        token = strtok(line, " ");
        if(strchr(token, ':')){
            if( getLabelLine(lblMap, token) != -1){ // label repeated
                fprintf(stderr, "label %s is repeated in line %d first found : %d\n", 
                        token, lineNo, getLabelLine(lblMap, token));
                return;
            }
            if(!prev){ // single label or first label in row of straight labels
                if(!addLabel(lblMap, token, lineNo - labelCount)){
                    freeTree(lblMap);
                    freeTree(idMap);
                    return;
                }
                prevLine = lineNo - labelCount; // last relative label line
            }
            else{if(!addLabel(lblMap, token, prevLine)){
                    freeTree(lblMap); // alloc error minus msg
                    freeTree(idMap);
                    return;}}
            ++labelCount;
            prev = 1;
        }
        else{ prev = 0;
            while(token = strtok(0, " ")){ // pulling uppercase variables
                if(idMExists(idMap, token)) {continue;}
                int brFlag = 0;
                for (int i = 0; i<strlen(token); ++i){
                    if(isupper(token[i])){
                        brFlag = 1;
                        break;
                    }
                }
                if(brFlag && token[strlen(token) - 1] != ':'){
                    char tmp[MAX_ID_SIZE];
                    newtemp(tmp);
                    if(!addMapId(idMap, token, tmp)){
                        freeTree(lblMap);
                        freeTree(idMap);
                        return;
                    }
                }
            }
        }
        ++lineNo;
    }
    fseek(tmpFile, 0, SEEK_SET); // reset file pointer
    while(fgets(line, MAX_LINE_LENGTH, tmpFile)){ // final replacement loop
        token = strtok(line, " ");

        if(!strchr(token, ':')){
            fprintf(outFile, "%s ", token);
            while((token = strtok(0, " "))) {
                char tmp[MAX_ID_SIZE];
                if (strchr(token, ':')){ //replacing labels 
                    int ind;
                    if(getLabelLine(lblMap, token) == -1){ // label not found
                        fprintf(stderr, "PANIC: label add not found please check label func!\n");
                        break;
                    }
                    sprintf(tmp, "%d", getLabelLine(lblMap, token));
                    token = tmp;
                }
                int ind;
                if (idMExists(idMap, token)){ //replaceing uppercase variables
                    token = getTmpId(idMap, token);
                }
                if (strcmp(token, "\n") != 0){
                    fprintf(outFile, "%s ", token);

                }
                else{
                    fprintf(outFile, "\n");
                }
            }
        }
        }

    fprintf(stderr, "Akthm Daas\n");
    fprintf(outFile, "Akthm Daas\n" );
    freeTree(idMap);
    freeTree(lblMap);
}
