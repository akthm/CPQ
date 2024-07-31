%code {

extern int yylex (void);
extern void yyerror (const char *);
extern FILE* yyout;



Node *root; // global variables search tree (name - type - is initialized)
NodeAdder * adder; // global linked list used in adding variables to tree
int ABORT = 0; //semantic error flag

void except(const char*msg, int isTerminal){ //any exception
    extern int yylineno;
    fprintf(stderr, "ERROR: on line: %d %s\n", yylineno, msg);
    ABORT = 1;
    if (isTerminal){
        extern FILE *yyin;
        extern FILE *yyout;
        freeTree(root);
        freeAdder(adder);
        fclose(yyout);
        fclose(yyin);
        exit(EXIT_FAILURE);
    }

}


/* to test if variable name conflicts with tmp(or anyone trying to fail this compiler)*/
int isTmpType(char *text){ 
    int len = strlen(text);
    if (len > 3){
        int a = text[0] == 't';
        int b = text[1] == 'm';
        int c = text[2] == 'p';
        if (a && b && c){
            for(int i=0; i < len; ++i){
                if(!isdigit(text[i])){
                    return 0;
                }
            }
            return 1;
        }
    }
    return 0;
}

 /* Semantic ID Checking  */
int testId(const char* id, int isinp){
    int index;
    extern int yylineno; 
    if(!idExists(root, id)){
        fprintf (stderr, "ID %s on line: %d - Undeclared ! declare it First !\n", id, yylineno);
        ABORT = 1;
        return 0;
        }
    if (!isinp && !idIsInit(root, id)){
        fprintf (stderr, "ID %s on line: %d -- Uninitialized ! !\n", id, yylineno);
        ABORT = 1;
        return 0;
    }
    return 1;
    }

/* To assign numbers input ExpType */
void setExp(char *target, NumVal val){
    if (val.type == N){
        sprintf(target, "%d", val.ival);
    }
    else {
        sprintf(target, "%f", val.fval);
    }
    if(strlen(target) > MAX_ID_SIZE){
        except("Number is too long!", 1);
    }
    }

/* assigning initialized ID*/
int initId( char *name){
        extern int yylineno;
        if (idExists(root, name)){
            idSetInit(root, name);
            return 1;
        }
        fprintf(stderr, "%s at line %d is Not Declared!\n", name, yylineno);
        ABORT =1;
        return 0;
    }


    /* opcodes indexes:
    * 0 - ==
    * 1 - !=
    * 2 - <
    * 3 - >
    * 4 - +
    * 5 - -
    * 6 - *
    * 7 - /
    *   Binary operations factory (R-type)
    */
int binOp(ExpType lh, ExpType rh, enum operator opr , ExpType *target){
        char *rOpcs[] = {"REQL", "RNQL", "RLSS", "RGRT", "RADD", "RSUB", "RMLT", "RDIV"};
        char *iOpcs[] = {"IEQL", "INQL", "ILSS", "IGRT", "IADD", "ISUB", "IMLT", "IDIV"};
        char **targetOps;
        char* fOp;
        char *lhRes = lh.result;
        char *rhRes = rh.result;
        char res[MAX_ID_SIZE];
        char tmp[MAX_ID_SIZE];
        newtemp(res);
       /******** casting and types checking *********/
        if (lh.type != rh.type) {
            newtemp(tmp);
            char castTemp[MAX_ID_SIZE];
            if (lh.type == N){
                gen(3, "ITOR", tmp, lhRes);
                lhRes = tmp;
            }
            else {
                gen(3, "ITOR", tmp, rhRes);
                rhRes = tmp;
            }
            target->type = FL;
            targetOps = rOpcs;
        }
        else {
            if(lh.type == N){
                targetOps = iOpcs;
                target->type = N;
                }
            else{
                targetOps = rOpcs;
                target->type = FL;
                }
        }
        /*******    op Factory      ********/
        ExpType lTmpRes; // logical temp result
            /* ExpType of 1 */
        ExpType oneT;
        strcpy(oneT.result, target->type == N ? "1" : "1.0");
        oneT.type = target->type;

        ExpType tmpT;
        switch(opr){
        /* logical ops */
        /* for jmpz here we make true -> 1 -- false -> 0  */
            case EQ:
                fOp = targetOps[0]; //  equal
                break;
            case NEQ:
                fOp = targetOps[1]; // not equal
                break;
            case GT:
                fOp = targetOps[3]; // greater
                break;
            case GE: // lh >= rh --> lh + 1 > rh
                strcpy(tmpT.result, lhRes);
                tmpT.type = target->type;
                binOp(tmpT, oneT, PLUS, &lTmpRes); // adding 1 to lh
                lhRes = lTmpRes.result;
                fOp = targetOps[3]; // greater
                break;
            case LT:
                fOp = targetOps[2]; // less
                break;
            case LE: // lh <= rh --> lh < rh + 1
                strcpy(tmpT.result, rhRes);
                tmpT.type = target->type;
                binOp(tmpT, oneT, PLUS, &lTmpRes); // adding 1 to rh
                rhRes = lTmpRes.result;
                fOp = targetOps[2]; // less
                break;
        /* arithmetic ops */
            case PLUS:
                fOp = targetOps[4];
                break;
            case MINUS:
                fOp = targetOps[5];
                break;
            case MUL:
                fOp = targetOps[6];
                break;
            case DIV:
                fOp = targetOps[7];
                break;
        }

        strcpy(target->result, res);
        gen (4, fOp, res, lhRes, rhRes);
        return 0;
    }
}



    /*
        NT -- NOT
        R -- OR
        ND -- AND 
    */
%code requires {
#include "utils.h"
enum operator {PLUS, MINUS, MUL, DIV, EQ, NEQ, GT, GE, LT, LE};
}


%union{
    NumVal numVal;
    char sval[MAX_ID_SIZE];
    enum operator op;
    Type T;
    ExpType exp; // struct for expression - factor - term
}

     /*  TOKENS   */
%token INT FLOAT BREAK IF INPUT OUTPUT WHILE ELSE CASE SWITCH  DEFAULT
%token <numVal> NUM
%token <op> MULOP RELOP ADDOP OR NOT AND
%token<T> CAST
%token <sval> ID

    /*  TYPES  */
%type<T> type
%type<exp> factor term expression
%type<sval> boolfactor boolterm boolexpr




%start program


%%

program             : declerations stmt_block {gen(1, "HALT");}
declerations        : declerations  decleration 
                    | %empty
decleration         : idlist ':' type ';' { loadNodesInto(root, adder, $3); }
type                : INT {$$ = N;} | FLOAT { $$ = FL;}
idlist              : idlist ',' ID  {addToAdder(adder, $3); }| ID {addToAdder(adder, $1); }
stmt                : assignment_stmt {} | input_stmt {} | output_stmt {} | if_stmt {}
                    | while_stmt {} | switch_stmt {} | break_stmt {} | stmt_block {}
assignment_stmt     : ID '=' expression ';' { if(initId($1)) {
                        char *res = $3.result;
                        Type idType = getIdType(root, $1);
                        if(idType !=  $3.type){
                            if(idType == FL){
                                char tmp[MAX_ID_SIZE];
                                newtemp(tmp);
                                gen(3, "ITOR", tmp, $3.result);
                                res = tmp;}
                            else {except("Cannot assign float to int", 0);}
                        }
                        gen(3, idType == N ? "IASN" : "RASN", $1, res);} }
input_stmt          : INPUT '(' ID ')' ';' {
                        if (testId($3, 1)) {
                            gen(2, getIdType(root, $3) == N ? "IINP" : "RINP", $3);
                            idSetInit(root, $3); }}
output_stmt         : OUTPUT '(' expression ')' ';' {gen(2,
                                                $3.type == N ? "IPRT" : "RPRT", $3.result); }
if_stmt             : IF
                    <sval>{newlabel($$);}             // $2 else label
                    <sval>{newlabel($$);}             // $3 exit label
                    '(' boolexpr ')' {
                            gen(3, "JMPZ", $<sval>2, $5); }
                     stmt {gen(2, "JUMP", $<sval>3); }
                     ELSE {gen(1, $<sval>2);}
                     stmt {gen(1, $<sval>3);}

while_stmt          : WHILE
                    <sval>{newlabel($$); gen(1, $$);} // $2 loop label
                    <sval>{newlabel($$);}             // $3 exit label
                    '(' boolexpr ')' {
                        gen(3, "JMPZ", $3, $5);
                     }
                     stmt {gen(2, "JUMP", $2); gen(1, $3);}
switch_stmt         : SWITCH '(' expression ')' '{' caselist
 DEFAULT ':' stmtlist '}'
caselist            : caselist CASE NUM ':' stmtlist | %empty
break_stmt          : BREAK ';'
stmt_block          : '{' stmtlist '}'
stmtlist            : stmtlist stmt {} | %empty {}
boolexpr            : boolexpr OR boolterm {
                            char tmp[MAX_ID_SIZE];
                            newtemp(tmp);
                            newtemp($$);
                            gen(4, "IADD", tmp, $1, $3);
                            gen(4, "IGRT", $$, tmp, "0"); // > 0
                        }
                    | boolterm {strcpy($$, $1);}
boolterm            : boolterm AND boolfactor {
                            char tmp[MAX_ID_SIZE];
                            newtemp(tmp);
                            newtemp($$);
                            gen(4, "IADD", tmp, $1, $3);
                            gen(4, "IEQL", $$, tmp, "2"); // == 2 (true true)
                        }

                    | boolfactor {strcpy($$, $1);}
boolfactor          : NOT '(' boolexpr ')' {
                            newtemp($$);
                            gen(4, "INQL", $$, $3, "1"); // $3 <> 1 inverts 1 -> 0 -- 0 -> 1
                        }

                    | expression RELOP expression {
                            ExpType lTmp;
                            binOp($1, $3, $2, &lTmp);
                            strcpy($$, lTmp.result);}

expression          : expression ADDOP term{
                            binOp($1, $3, $2, &$$);}

                    | term {strcpy($$.result, $1.result); $$.type = $1.type;}

term                : term MULOP factor {
                            binOp($1, $3, $2, &$$);
                        }
                            |factor {strcpy($$.result, $1.result); $$.type = $1.type;}

factor              :  '(' expression ')'  { strcpy($$.result, $2.result); $$.type = $2.type;}|
                 CAST '(' expression ')' {
                if($1 != $3.type){
                    newtemp($$.result); // creating new temp
                    gen(3, $1 == N ? "RTOI" : "ITOR", $$.result, $3.result); }
                    else{strcpy($$.result, $3.result);}
                $$.type = $1;
                }
                    | ID { if (testId($1, 0)){
                                $$.type = getIdType(root, $1);
                            }
                            else {
                                $$.type = N; //there will be semantic error
                            }
                        if(strlen($1) >= MAX_ID_SIZE){
                            except("ID name is too long!", 1);
                        }
                        if(isTmpType($1)){ // variable is tmp<num>
                            $1[0] = 'T'; // will be replaced with tmp at final screening
                        }
                        strcpy($$.result, $1); }
                    | NUM { $$.type = $1.type; char tmp[MAX_ID_SIZE]; setExp(tmp, $1); strcpy($$.result, tmp);  }


%%






/* called by yyparse() whenever a syntax error is detected */
void yyerror (const char *s)
{
  extern int yylineno; // defined by flex
  extern char *yytext; // defined by flex
  
  fprintf (stderr, "Error on line %d:%s --> %s\n", yylineno,s, yytext);
  ABORT = 1;
}
