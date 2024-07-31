%{
#include "cpq.tab.h"
%}

%option yylineno
%option noyywrap
%x C_COMMENT

%%  

\/\/.*  {  }
"/*"            { BEGIN(C_COMMENT); }
<C_COMMENT>"*/" { BEGIN(INITIAL); }
<C_COMMENT>\n   { }
<C_COMMENT>.    { }

    /* keywords */
break      { return BREAK; }
case       { return CASE; }
default    { return DEFAULT; }
if         {return IF; }
else       { return ELSE; }
float      {return FLOAT; }
input      {return INPUT;}
int        { return INT; }
output     { return OUTPUT; }
switch     { return SWITCH; }
while      { return WHILE; } 
    /* symbols */
\(          { return '('; }
\)          { return ')'; }
\{          { return '{'; }
\}          { return '}'; }
,           { return ','; }
:           { return ':'; }
;           { return ';'; }
=           { return '='; }
    /* operations */
[!=]=       { yylval.op = yytext[0] == '=' ? EQ : NEQ; return RELOP;}
[<>]        { yylval.op = yytext[0] == '<' ? LT : GT; return RELOP;}
[<>]=      { yylval.op = yytext[0] == '<' ? LE : GE; return RELOP;}
[+-]        {yylval.op = yytext[0] == '+' ? PLUS : MINUS; return ADDOP; }
[*/]        {yylval.op = yytext[0] == '*' ? MUL : DIV; return MULOP;}
\|\|        { return OR;}
&&          { return AND;}
!           { return NOT;}
    /* aditional */
static_cast\<(int|float)\>  { yylval.T = strstr(yytext, "int") ? N : FL; return CAST;}
[a-zA-Z][A-Za-z0-9]*        {strcpy(yylval.sval, yytext); return ID;}
[0-9]+(\.[0-9]*)?           {
    if(strchr(yytext, '.')) {
        yylval.numVal.fval = atof(yytext);
        yylval.numVal.type = FL;
    }
    else {
        yylval.numVal.ival = atoi(yytext);
        yylval.numVal.type = N;
    }
    return NUM; }

(\s|\r|\t)* { }
[ ]*        { }
\n*         { }
%%