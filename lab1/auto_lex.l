
%{
#include "token.h"

%}
SINGLEDIGIT [0-9]
DIGITS [0-9]{SINGLEDIGIT}*
INTEGER {DIGITS}
HEXINT (0[xX][a-fAF0-9]+)
DECIMAL	 {INTEGER}"."{DIGITS}
FLOAT ({INTEGER}|{DECIMAL})(e|E){INTEGER}
SINGLELETTER [A-Za-z]
IDENTIFY ({SINGLELETTER}|_)({SINGLELETTER}|{SINGLEDIGIT}|_)*
OPT "+"|"-"|"*"|"/"|"+="|"-="|"*="|"/="|">"|"<"|"="|">="|"<="|"=="|"++"|"--"|"%"|"&&"|"||"|"!="|"!"|"&"|"|"
BRACKET "("|")"|"["|"]"|"{"|"}"|","|";"|"\""|"\'"
TYPEIDENTIFY %{IDENTIFY}|&{IDENTIFY}
COMMENT \/\/(.)*|\/\*(.|\n)*\*\/

%%

\n {}
{COMMENT} {}
"float" {printf("<%d, KEYWORD>\n",(int)Y_FLOAT);}
"int" {printf("<%d, KEYWORD>\n",(int)Y_INT);}
"void" {printf("<%d, KEYWORD>\n", (int)Y_VOID);}
"const" {printf("<%d, KEYWORD>\n", (int)Y_CONST);}
"if" {printf("<%d, KEYWORD>\n", (int)Y_IF);}
"else" {printf("<%d, KEYWORD>\n", (int)Y_ELSE);}
"while" {printf("<%d, KEYWORD>\n", (int)Y_WHILE);}
"break" {printf("<%d, KEYWORD>\n", (int)Y_BREAK);}
"continue" {printf("<%d, KEYWORD>\n", (int)Y_CONTINUE);}
"return" {printf("<%d, KEYWORD>\n", (int)Y_RETURN);}
{BRACKET} {
    if(strcmp(yytext,"(")==0)printf("<%d, SYMBOL>\n", (int)Y_LPAR);
    else if(strcmp(yytext,")")==0)printf("<%d, SYMBOL>\n", (int)Y_RPAR);
    else if(strcmp(yytext,"[")==0)printf("<%d, SYMBOL>\n", (int)Y_LSQUARE);
    else if(strcmp(yytext,"]")==0)printf("<%d, SYMBOL>\n", (int)Y_RSQUARE);
    else if(strcmp(yytext,"{")==0)printf("<%d, SYMBOL>\n", (int)Y_LBRACKET);
    else if(strcmp(yytext,"}")==0)printf("<%d, SYMBOL>\n", (int)Y_RBRACKET);
    else if(strcmp(yytext,",")==0)printf("<%d, SYMBOL>\n", (int)Y_COMMA);
    else if(strcmp(yytext,";")==0)printf("<%d, SYMBOL>\n", (int)Y_SEMICOLON);
    }
{OPT} {if(strcmp(yytext,"+")==0)printf("<%d, OPERATOR>\n", (int)Y_ADD);
    else if(strcmp(yytext,"-")==0)printf("<%d, OPERATOR>\n", (int)Y_SUB);
    else if(strcmp(yytext,"*")==0)printf("<%d, OPERATOR>\n", (int)Y_MUL);
    else if(strcmp(yytext,"/")==0)printf("<%d, OPERATOR>\n", (int)Y_DIV);
    else if(strcmp(yytext,"%")==0)printf("<%d, OPERATOR>\n", (int)Y_MODULO);
    else if(strcmp(yytext,"<")==0)printf("<%d, OPERATOR>\n", (int)Y_LESS);
    else if(strcmp(yytext,"<=")==0)printf("<%d, OPERATOR>\n", (int)Y_LESSEQ);
    else if(strcmp(yytext,">")==0)printf("<%d, OPERATOR>\n", (int)Y_GREAT);
    else if(strcmp(yytext,">=")==0)printf("<%d, OPERATOR>\n", (int)Y_GREATEQ);
    else if(strcmp(yytext,"!=")==0)printf("<%d, OPERATOR>\n", (int)Y_NOTEQ);
    else if(strcmp(yytext,"==")==0)printf("<%d, OPERATOR>\n", (int)Y_EQ);
    else if(strcmp(yytext,"!")==0)printf("<%d, OPERATOR>\n", (int)Y_NOT);
    else if(strcmp(yytext,"&&")==0)printf("<%d, OPERATOR>\n", (int)Y_AND);
    else if(strcmp(yytext,"||")==0)printf("<%d, OPERATOR>\n", (int)Y_OR);
    else if(strcmp(yytext,"=")==0)printf("<%d, OPERATOR>\n", (int)Y_ASSIGN);
    }
    
{TYPEIDENTIFY} {}
{HEXINT} {int a;sscanf(yytext, "%x", &a);printf("<%d, %d>\n", (int)num_INT,a);}
{INTEGER} {printf("<%d, %s>\n", (int)num_INT,yytext);}
{DECIMAL} {printf("<%d, %s>\n", (int)num_FLOAT,yytext);}
{FLOAT} {printf("<%d, %s>\n", (int)num_FLOAT,yytext);}
{IDENTIFY} {printf("<%d, %s>\n", (int)Y_ID,yytext);}
. {}
%%
int main(){
    FILE *f;
	f = fopen("input.c", "r");
	yyin = f;
    yylex();
    //yylex();
    return 0;
}
int yywrap(){
    return 1;
}
