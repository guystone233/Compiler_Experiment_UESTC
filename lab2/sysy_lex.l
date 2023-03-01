%{

enum yytokentype{
	IntConst = 258,
	EOL,
	Ident,
	INT,		 // int
	VOID,		 // void
	CONST,		 // const
	IF,			 // if
	ELSE,		 // else
	WHILE,		 // while
	BREAK,		 // break
	CONTINUE,	 // continue
	RETURN,		 // return
	LESSEQ,		 // <=
	GREATEQ,	 // >=
	NOTEQ,		 // !=
	EQ,		 	 // ==
	AND, 		 // &&
	OR,			 // ||
};

int yylval;

%}

INTERGER   [0-9]
OCTALCONS  0[0-7]+
HEXCONS    0[xX][0-9a-fA-F]+
NOTE_S     \/\/(.)*\n
NOTE_M     \/\*(.|\n)*?\*\/
IDENTIFIER [_a-zA-Z][_a-zA-Z0-9]*


%%

"int"                       {return INT;}
"continue"                  {return CONTINUE;}
"const"                     {return CONST;}
"else"                      {return ELSE;}
"if"                        {return IF;}
"return"                    {return RETURN;}
"void"                      {return VOID;}
"while"                     {return WHILE;}
"break"                     {return BREAK;}

"("   |
")"   |
"{"   |
"}"   |
"["   |
"]"   |
","   |
";"   |
"+"   |
"-"   |
"*"   |
"/"   |
"%"   |
"<"   |
">"   |
"!"   |
"="   |
"q"                         {return yytext[0];}

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

{INTERGER}+"."*{INTERGER}* 	|
{OCTALCONS}                 |
{HEXCONS}                   { yylval = atoi(yytext); return IntConst;}

{IDENTIFIER}                {return Ident;}

%%

int yywrap(){
    return 1;
}

void myUnput(char* yytext, int len){
	int i;
	char *tmpStore =  strdup(yytext);
	for(i = len - 1; i >= 0; i--){
		unput(tmpStore[i]);
	}
	unput(' ');
}