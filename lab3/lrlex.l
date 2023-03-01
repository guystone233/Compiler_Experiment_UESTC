%{

#include "ast.h"
#include <string.h>
#include "lrparser.tab.h"

%}

INTERGER   [0-9]
OCTALCONS  0[0-7]+
HEXCONS    0[xX][0-9a-fA-F]+
NOTE_S     \/\/(.)*\n
NOTE_M     \/\*(.|\n)*?\*\/
IDENTIFIER [_a-zA-Z][_a-zA-Z0-9]*

%%

"("                         {return '(';}   
")"                         {return ')';}   
"{"                         {return '{';}   
"}"                         {return '}';}   
"["                         {return '[';}   
"]"                         {return ']';}   
","                         {return ',';}   
";"                         {return ';';}   
"+"                         {return '+';}   
"-"                         {return '-';}   
"*"                         {return '*';} 
"/"                         {return '/';} 
"%"                         {return '%';}  
"<"                         {return '<';}  
">"                         {return '>';}   
"!"                         {return '!';} 
"="                         {return '=';}
"int"                       {return INT;}
"continue"                  {return CONTINUE;}
"const"                     {return CONST;}
"else"                      {return ELSE;}
"if"                        {return IF;}
"return"                    {return RETURN;}
"void"                      {return VOID;}
"while"                     {return WHILE;}
"break"                     {return BREAK;}
"<="                        {return LESSEQ;}
">="                        {return GREATEQ;}
"!="                        {return NOTEQ;}
"=="                        {return EQ;}
"&&"                        {return AND;}
"||"                        {return OR;}         
" "                         {}
"\t"                        {}
"\n"                        {}
{NOTE_S}*                   {}
{NOTE_M}*                   {}
{INTERGER}+"."*{INTERGER}*|{OCTALCONS}|{HEXCONS}   {int a;sscanf(yytext, "%d", &a);yylval.iVal = a; return num_INT;}
{IDENTIFIER}                {strcpy(yylval.strValue, yytext); return ID;}

%%

int yywrap(){
    return 1;
}
