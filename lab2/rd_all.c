#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum yytokentype
{
	IntConst = 258,
	EOL,
	Ident,
	INT,	  // int
	VOID,	  // void
	CONST,	  // const
	IF,		  // if
	ELSE,	  // else
	WHILE,	  // while
	BREAK,	  // break
	CONTINUE, // continue
	RETURN,	  // return
	LESSEQ,	  // <=
	GREATEQ,  // >=
	NOTEQ,	  // !=
	EQ,		  // ==
	AND,	  // &&
	OR,		  // ||
};

void myUnput(char *yytext, int len);
extern int yylex();
extern int yylval;
extern int yyleng;
extern char *yytext;
extern FILE *yyin;

int tok;

typedef struct _ast ast;
typedef struct _ast *past;
struct _ast
{
	int ivalue;
	char *strValue;
	char *nodeType;
	past next;
	past left;
	past right;
};

void advance();
void showAst();
int judgeDeclAndFuncDef();
past astCompUnit();
past astDecl();
past astConstDecl();
past astBType();
past astConstDef();
past astConstInitVal();
past astVarDecl();
past astVarDef();
past astInitVal();
past astFuncDef();
past astFuncType();
past astFuncFParams();
past astFuncFParam();
past astBlock();
past astBlockItem();
past astStmt();
past astExp();
past astCond();
past astLVal();
past astPrimaryExp();
past astNumber();
past astUnaryExp();
past astFuncRParams();
past astMulExp();
past astAddExp();
past astRelExp();
past astEqExp();
past astLAndExp();
past astLOrExp();
past astConstExp();

int main(int argc, char **argv)
{
	if (argc > 2)
	{
		printf("argcs too many!.\n");
		return 0;
	}
	if (argc == 2)
	{
		FILE *f;
		f = fopen(argv[1], "r");
		setbuf(stdout, NULL);
		yyin = f;
	}
	else
	{
		yyin = fopen("./expr.txt", "r");
		setbuf(stdout, NULL);
	}

	advance();
	past rr = astCompUnit();
	showAst(rr, 0);

	return 0;
}

void advance()
{
	tok = yylex();
	// printf("tok: %s\n", yytext);
}

int judgeDeclAndFuncDef()
{
	// Decl->1 FuncDef->2 else->-1
	//  Decl -> const || int   int -> Ident
	//  FuncDef -> void || int  int -> Ident Ident -> (
	if (tok == CONST)
		return 1;
	else if (tok == VOID)
		return 2;
	else if (tok == INT)
	{
		char *s1 = strdup(yytext);
		int l1 = yyleng;
		advance();
		if (tok == Ident)
		{
			char *s2 = strdup(yytext);
			int l2 = yyleng;
			advance();
			if (tok == '(')
			{
				myUnput(yytext, yyleng);
				myUnput(s2, l2);
				myUnput(s1, l1);
				advance();
				return 2;
			}
			else
			{
				myUnput(yytext, yyleng);
				myUnput(s2, l2);
				myUnput(s1, l1);
				advance();
				return 1;
			}
		}
		else
			return -1;
	}
}

past newAstNode()
{
	past node = malloc(sizeof(ast));
	if (node == NULL)
	{
		printf("run out of memory.\n");
		exit(0);
	}
	memset(node, 0, sizeof(ast));
	node->strValue = NULL;
	node->nodeType = NULL;
	return node;
}

past newAstNode2(char *strVal, char *nodeType, past left, past right, past next)
{
	past node = (past)malloc(sizeof(ast));
	if (node == NULL)
	{
		printf("run out of memory.\n");
		exit(0);
	}
	memset(node, 0, sizeof(ast));
	node->strValue = strVal;
	node->nodeType = nodeType;
	node->next = next;
	node->left = left;
	node->right = right;
	return node;
}

past newNum(int value, past next)
{
	past var = newAstNode();
	var->nodeType = "intValue";
	var->ivalue = value;
	var->next = next;
	return var;
}

past newType(int type)
{
	past var = newAstNode();
	var->nodeType = "type";
	switch (type)
	{
	case VOID:
		var->strValue = "VOID";
		break;
	case INT:
		var->strValue = "INT";
		break;
	case CONST:
		var->strValue = "CONST";
		break;
	default:
		break;
	}
	var->left = NULL;
	var->right = NULL;
	var->next = NULL;
}

past newIdent(char *strVal, past next)
{
	past var = newAstNode();
	var->nodeType = "varDecl";
	var->strValue = strVal;
	var->next = next;
	return var;
}

past newExpr(int oper, past left, past right, past next)
{
	past var = newAstNode();
	switch (oper)
	{
	case LESSEQ:
		var->strValue = "<=";
		break;
	case GREATEQ:
		var->strValue = ">=";
		break;
	case NOTEQ:
		var->strValue = "!=";
		break;
	case EQ:
		var->strValue = "==";
		break;
	case AND:
		var->strValue = "&&";
		break;
	case OR:
		var->strValue = "||";
		break;
	default:
		var->ivalue = oper;
		var->strValue = "@";
		break;
	}
	var->nodeType = "expr";
	// var->ivalue = oper;
	var->left = left;
	var->right = right;
	var->next = next;
	return var;
}

void showAst(past node, int nest)
{
	if (node == NULL)
		return;
	int i = 0;
	for (i = 0; i < nest; i++)
		printf("  ");
	if (strcmp(node->nodeType, "intValue") == 0)
		printf("%s %d\n", node->nodeType, node->ivalue);
	else if (strcmp(node->nodeType, "expr") == 0)
	{
		if (strcmp(node->strValue, "@") == 0)
			printf("%s '%c'\n", node->nodeType, (char)node->ivalue);
		else
			printf("%s %s\n", node->nodeType, node->strValue);
	}
	else if (strcmp(node->nodeType, "list") == 0)
		printf("%s %d\n", node->nodeType, node->ivalue);
	else
		printf("%s %s\n", node->nodeType, node->strValue);

	showAst(node->left, nest + 1);
	showAst(node->right, nest + 1);
	showAst(node->next, nest);
}

// CompUnit:  [ CompUnit ] ( Decl | FuncDef )
past astCompUnit()
{
	past root = newAstNode2("", "CompUnit", NULL, NULL, NULL);
	past tmp;
	int type;
	if (tok == CONST || tok == VOID || tok == INT)
	{
		// Decl->1 FuncDef->2 else->-1
		type = judgeDeclAndFuncDef();
		if (type == 1)
		{
			past sentence = astDecl();
			root->left = sentence;
			tmp = sentence;
		}
		else if (type == 2)
		{
			past sentence = astFuncDef();
			root->left = sentence;
			tmp = sentence;
		}
		else
		{
			return NULL;
		}
	}
	while (tok == CONST || tok == VOID || tok == INT)
	{
		type = judgeDeclAndFuncDef();
		if (type == 1)
		{
			tmp->next = astDecl();
			tmp = tmp->next;
		}
		else if (type = 2)
		{
			tmp->next = astFuncDef();
			tmp = tmp->next;
		}
		else
		{
			break;
		}
	}
	return root;
}

// Decl: ConstDecl | VarDecl
past astDecl()
{
	past root = newAstNode2("", "decl", NULL, NULL, NULL);
	if (tok == CONST)
	{
		past cons = astConstDecl();
		root->left = cons;
		return root;
	}
	else if (tok == INT)
	{
		past intnode = astVarDecl();
		root->left = intnode;
		return root;
	}
	else
	{
		printf("An Error occurred in astDecl\n");
		return NULL;
	}
}

// ConstDecl: 'const' BType ConstDef {','ConstDef} ';'
past astConstDecl()
{
	past root = newAstNode2("", "constDecl", NULL, NULL, NULL);
	if (tok == CONST)
	{
		past t = newType(tok);
		advance();
		if (tok == INT)
		{
			t->next = astBType();
			if (tok == Ident)
			{
				past consdef = astConstDef();
				root->right = consdef;
				past tmp = root->right;
				while (tok == ',')
				{
					advance();
					past consdef = astConstDef();
					tmp->next = consdef;
					tmp = tmp->next;
				}
				if (tok == ';')
				{
					advance();
					root->left = t;
					return root;
				}
				else
				{
					printf("An Error occurred in astConstDecl: no ; \n");
				}
			}
			else
			{
				printf("An Error occurred in astConstDecl\n");
				return NULL;
			}
		}
		else
		{
			printf("An Error occurred in astConstDecl\n");
			return NULL;
		}
	}
	else
	{
		printf("An Error occurred in astConstDecl\n");
		return NULL;
	}
}

// BType: 'int'
past astBType()
{
	past t = NULL;
	if (tok == INT)
	{
		t = newType(tok);
		advance();
	}
	else
		printf("An Error occurred in astBType\n");
	return t;
}

// ConstDef: Ident { '[' ConstExp ']' } '=' ConstInitVal
past astConstDef()
{
	past root = newAstNode2("", "constDef", NULL, NULL, NULL);
	past tmp;
	if (tok == Ident)
	{
		past iden = newIdent(strdup(yytext), NULL);
		root->left = iden;
		advance();
		if (tok == '[')
		{
			advance();
			past consExp = astConstExp();
			iden->left = consExp;
			tmp = consExp;
			if (tok == ']')
				advance();
			else
			{
				printf("An Error occurred in astConstDef\n");
				return NULL;
			}
		}
		while (tok == '[')
		{
			advance();
			past consExp = astConstExp();
			tmp->next = consExp;
			tmp = tmp->next;
			if (tok == ']')
				advance();
			else
			{
				printf("An Error occurred in astConstDef\n");
				return NULL;
			}
		}
		if (tok == '=')
		{
			advance();
			past consInit = astConstInitVal();
			root->right = consInit;
		}
		return root;
	}
	else
	{
		printf("An Error occurred in astConstDef\n");
		return NULL;
	}
}

// ConstInitVal: ConstExp |  '{' [ ConstInitVal { ',' ConstInitVal } ] '}'
past astConstInitVal()
{
	past list = newAstNode2("", "list", NULL, NULL, NULL);
	past root = newAstNode2("", "constInitVal", list, NULL, NULL);
	list->ivalue = 0;
	past tmp;
	if (tok == '{')
	{
		advance();
		if (tok != '}')
		{
			past son = astConstInitVal();
			list->left = son;
			tmp = son;
			list->ivalue++;
			while (tok == ',')
			{
				advance();
				past son = astConstInitVal();
				tmp->next = son;
				tmp = tmp->next;
				list->ivalue++;
			}
			if (tok == '}')
			{
				advance();
				return root;
			}
		}
		else
		{
			list->left = newAstNode2("", "noexp", NULL, NULL, NULL);
			list->ivalue++;
			return root;
		}
	}
	else
	{
		list->ivalue++;
		list->left = astConstExp();
		if (list->left != NULL)
			return root;
		else
		{
			printf("An Error occurred in astConstInitVal\n");
			return NULL;
		}
	}
}

// VarDecl: BType VarDef {',' VarDef}';'
past astVarDecl()
{
	past tmp = NULL;
	if (tok == INT)
	{
		past type = newType(tok);
		past item = newAstNode2("", "list", NULL, NULL, NULL);
		past root = newAstNode2("", "varDecl", type, item, NULL);
		root->right->ivalue = 0;
		advance();
		if (tok == Ident)
		{
			past var = astVarDef();
			item->ivalue++;
			item->left = var;
			tmp = var;
		}
		while (tok == ',')
		{
			advance();
			past var = astVarDef();
			item->ivalue++;
			tmp->next = var;
			tmp = tmp->next;
		}
		if (tok == ';')
		{
			advance();
			return root;
		}
		else
		{
			printf("An Error occurred in astVarDecl  ; needed\n");
			return NULL;
		}
	}
	else
	{
		printf("An Error occurred in astVarDecl\n");
		return NULL;
	}
}

// VarDef: Ident {'['ConstExp']'} | Ident {'['ConstExp']'} '=' InitVal
past astVarDef()
{
	if (tok == Ident)
	{
		past root = newAstNode2(".", "varDef", newAstNode2(strdup(yytext), "varDecl", NULL, NULL, NULL), NULL, NULL);
		advance();
		past listnode = newAstNode2("", "list", NULL, NULL, NULL);
		root->left->left = listnode;
		root->left->left->ivalue = 0;
		past temp = NULL;
		if (tok == '[')
		{
			root->left->nodeType = "arrayDecl";
			advance();
			past head = astConstExp();
			root->left->left->left = head;
			root->left->left->ivalue++;
			past temp = head;
			if (tok == ']')
			{
				advance();
				while (tok == '[')
				{
					advance();
					past first = astConstExp();
					temp->next = first;
					temp = temp->next;
					root->left->left->ivalue++;
					if (tok == ']')
					{
						advance();
					}
				}
				if (tok == '=')
				{
					advance();
					past init = astInitVal();
					root->right = init;
					return root;
				}
				else
				{
					return root;
				}
			}
		}
		else if (tok == '=')
		{
			advance();
			past rightnode = astInitVal();
			root->right = rightnode;
			return root;
		}
		else
		{
			return root;
		}
	}
}

// InitVal: Exp | '{' [ InitVal { ',' InitVal } ] '}'
past astInitVal()
{
	past list = newAstNode2("", "list", NULL, NULL, NULL);
	past root = newAstNode2("", "initVal", list, NULL, NULL);
	list->ivalue = 0;
	past tmp;
	if (tok == '{')
	{
		advance();
		if (tok != '}')
		{
			past son = astInitVal();
			list->left = son;
			tmp = son;
			list->ivalue++;
			while (tok == ',')
			{
				advance();
				past son = astInitVal();
				tmp->next = son;
				tmp = tmp->next;
				list->ivalue++;
			}
			if (tok == '}')
			{
				advance();
				return root;
			}
		}
		else
		{
			list->left = newAstNode2("", "noexp", NULL, NULL, NULL);
			list->ivalue++;
			return root;
		}
	}
	else
	{
		list->ivalue++;
		list->left = astExp();
		if (list->left != NULL)
			return root;
		else
		{
			printf("An Error occurred in astConstInitVal\n");
			return NULL;
		}
	}
}

// FuncDef: FuncType Ident '(' [FuncFParams] ')' Block
past astFuncDef()
{
	past root = newAstNode();
	root->nodeType = "funcDef";
	if (tok == VOID || tok == INT)
	{
		past type = astFuncType();
		root->left = type;
		if (tok == Ident)
		{
			past iden = newIdent(strdup(yytext), NULL);
			type->left = iden;
			advance();
			if (tok == '(')
			{
				advance();
				if (tok != ')')
				{
					past funcFP = astFuncFParams();
					iden->left = funcFP;
					advance();
				}
				else
				{
					advance();
				}
				past block = astBlock();
				root->right = block;
				return root;
			}
			else
			{
				printf("An Error occurred in astFuncDef\n");
				return NULL;
			}
		}
		else
		{
			printf("An Error occurred in astFuncDef\n");
			return NULL;
		}
	}
	else
	{
		printf("An Error occurred in astFuncDef\n");
		return NULL;
	}
}

// FuncType:  'void' | 'int'
past astFuncType()
{
	past t = NULL;
	if (tok == VOID || tok == INT)
	{
		t = newType(tok);
		advance();
	}
	else
		printf("An Error occurred in astFuncType\n");
	return t;
}

// FuncFParams: FuncFParam { ',' FuncFParam }
past astFuncFParams()
{
	int list = 0;
	past root = newAstNode2("", "FuncFParams", newAstNode2("", "list", NULL, NULL, NULL), NULL, NULL);
	root->left->ivalue = list;
	past head = astFuncFParam();
	root->left->left = head;
	root->left->ivalue++;
	while (tok == ',')
	{
		advance();
		past first = astFuncFParam();
		root->left->ivalue++;
		head->next = first;
		head = head->next;
	}
	return root;
}

// FuncFParam: BType Ident ['[' ']' { '[' Exp ']' }]
past astFuncFParam()
{
	int list = 0;
	past leftint = astBType();
	if (tok == Ident)
	{
		// past root = newAstNode2("","FuncFParam",leftint,newAstNode2(strdup(yytext),"Ident",NULL,NULL,NULL),NULL);
		past root = newAstNode2("", "FuncFParam", leftint, newIdent(strdup(yytext), NULL), NULL);
		advance();
		past listnode = newAstNode2("", "list", NULL, NULL, NULL);
		root->right->left = listnode;
		root->right->left->ivalue = list;
		past tmp = NULL;
		if (tok == '[')
		{
			advance();
			if (tok == ']')
			{
				past head = newAstNode2("", "noexp", NULL, NULL, NULL);
				root->right->left->left = head;
				root->right->left->ivalue++;
				tmp = head;
				advance();
				while (tok == '[')
				{
					advance();
					past first = astExp();
					tmp->next = first;
					tmp = tmp->next;
					root->right->left->ivalue++;
					if (tok == ']')
					{
						advance();
					}
				}
				return root;
			}
		}
		else
		{
			return root;
		}
	}
}

// Block:  '{' { BlockItem } '}'
past astBlock()
{
	past list = newAstNode2("", "list", NULL, NULL, NULL);
	list->ivalue = 0;
	past block = newAstNode2("", "blockDecl", NULL, NULL, NULL);
	list->left = block;
	past tmp;
	if (tok == '{')
	{
		advance();
		if (tok != '}')
		{
			list->ivalue++;
			past item = astBlockItem();
			block->left = item;
			tmp = item;
		}
		while (tok != '}')
		{
			list->ivalue++;
			past item = astBlockItem();
			tmp->next = item;
			tmp = tmp->next;
		}
	}
	advance();
	return list;
}

// BlockItem: Decl | Stmt
past astBlockItem()
{
	if (tok == CONST || tok == INT)
	{
		//		advance();
		return astDecl();
	}
	else
	{
		return astStmt();
	}
}

// Stmt: LVal '=' Exp ';'
//       | [Exp] ';'
//       | Block
//       | 'if' '( Cond ')' Stmt [ 'else' Stmt ]
//       | 'while' '(' Cond ')' Stmt
//       | 'break' ';'
//       | 'continue' ';'
//       | 'return' [Exp] ';'
past astStmt()
{
	past root = NULL;
	if (tok == Ident)
	{
		char *s1 = strdup(yytext);
		int l1 = yyleng;
		advance();
		//  [Exp] ';'
		if (tok == '(')
		{
			myUnput(yytext, yyleng);
			myUnput(s1, l1);
			advance();
			past root = newAstNode2("", "expStmt", NULL, NULL, NULL);
			if (root->left = astExp())
			{
				if (tok == ';')
				{
					advance();
					return root;
				}
			}
		}

		// LVal '=' Exp ';'
		myUnput(yytext, yyleng);
		myUnput(s1, l1);
		advance();
		root = astLVal();
		if (root != NULL)
		{
			if (tok == '=')
			{
				advance();
				past exp = astExp();
				past head = newAstNode2("", "AssignStmt", root, exp, NULL);
				if (tok == ';')
				{
					advance();
					return head;
				}
			}
		}
	}
	else if (tok == IF)
	{
		int list = 1;
		root = newAstNode();
		root->nodeType = "IfStmt";
		root->right = newAstNode2("", "list", NULL, NULL, NULL);
		root->right->ivalue = list;
		advance();
		if (tok == '(')
		{
			advance();
			past cond = astCond();
			root->left = cond;
			if (tok == ')')
			{
				advance();
				if (root->right->left = astStmt())
				{
					if (tok == ELSE)
					{
						advance();
						if (root->right->right = astStmt())
						{
							root->right->ivalue++;
							return root;
						}
					}
					//					advance();
					return root;
				}
			}
			else
				printf("An Error occurred in astStmt\n");
		}
		else
			printf("An Error occurred in astStmt\n");
	}
	else if (tok == WHILE)
	{
		root = newAstNode2("", "WhileStmt", NULL, NULL, NULL);
		advance();
		if (tok == '(')
		{
			advance();
			past cond = astCond();
			root->left = cond;
			if (tok == ')')
			{
				advance();
				if (root->right = astStmt())
				{
					return root;
				}
				else
					printf("An Error occurred in astStmt\n");
			}
			else
				printf("An Error occurred in astStmt\n");
		}
		else
			printf("An Error occurred in astStmt\n");
	}
	else if (tok == BREAK)
	{
		root = newAstNode2("", "BreakStmt", NULL, NULL, NULL);
		advance();
		if (tok == ';')
		{
			advance();
			return root;
		}
	}
	else if (tok == CONTINUE)
	{
		root = newAstNode2("", "ContinueStmt", NULL, NULL, NULL);
		advance();
		if (tok == ';')
		{
			advance();
			return root;
		}
	}
	else if (tok == RETURN)
	{
		root = newAstNode2("", "ReturnStmt", NULL, NULL, NULL);
		advance();
		root->left = astExp();
		if (tok == ';')
		{
			advance();
			return root;
		}
	}
	else if (tok == '{')
	{
		return astBlock();
	}
	else if (tok == ';')
	{
		root = newAstNode2("", "EmptyStmt", NULL, NULL, NULL);
		advance();
		return root;
	}
	else
	{
		return NULL;
	}
}

// Exp: AddExp
past astExp()
{
	past a = astAddExp();
	return a;
}

// Cond: LOrExp
past astCond()
{
	past a = astLOrExp();
	return a;
}

// LVal: Ident {'[' Exp ']'}
past astLVal()
{
	int flag = 1;
	if (tok != Ident)
		return NULL;
	// past LVal = newIdent(strdup(yytext), NULL);
	// past LVal = newAstNode();
	past root = newIdent(strdup(yytext), NULL);
	past tmp;
	advance();
	while (tok == '[')
	{
		advance();
		past exp = astExp();
		if (flag == 1)
		{
			root->left = exp;
			root->nodeType = "arrayDecl";
			tmp = exp;
			flag++;
		}
		else
		{
			tmp->next = exp;
			tmp = tmp->next;
		}
		if (tok == ']')
			advance();
		else
		{
			printf("An Error occurred in LVal\n");
			return NULL;
		}
	}
	return root;
}

// PrimaryExp: '(' Exp ')' | LVal | Number
past astPrimaryExp()
{
	if (tok == '(')
	{
		advance();
		past pri = astExp();
		if (tok == ')')
		{
			advance();
			return pri;
		}
		else
			printf("An Error occurred in astPrimaryExp\n");
	}
	else if (tok == Ident)
		return astLVal();
	else if (tok == IntConst)
		return astNumber();
	else
		printf("An Error occurred in astPrimaryExp\n");
	return NULL;
}

// Number: IntConst
past astNumber()
{
	if (tok == IntConst)
	{
		past num = newNum(yylval, NULL);
		advance();
		return num;
	}
	return NULL;
}

// UnaryExp: PrimaryExp | Ident '(' [FuncRParams] ')' | ('+' | '-' | '!') UnaryExp
past astUnaryExp()
{
	past ue = NULL;
	if (tok == '+' || tok == '-' || tok == '!')
	{
		while (tok == '+' || tok == '-' || tok == '!')
		{
			int oper = tok;
			advance();
			ue = astUnaryExp();
			ue = newExpr(oper, NULL, ue, NULL);
		}
		advance();
		return ue;
	}
	else if (tok == Ident)
	{
		char *s1 = strdup(yytext);
		int l1 = yyleng;
		ue = newIdent(strdup(yytext), NULL);
		advance();
		if (tok == '(')
		{
			advance();
			if (tok == ')')
			{
				advance();
				return ue;
			}
			else
			{
				past root = newAstNode();
				root->nodeType = "funcDecl";
				root->left = ue;
				past funcRP = astFuncRParams();
				root->right = funcRP;
				if (tok == ')')
				{
					advance();
					return root;
				}
				else
					printf("An Error occurred in astUnaryExp\n");
			}
			return NULL;
		}
		// PrimaryExp.LVal
		else
		{
			if (tok == ')')
				return ue;
			myUnput(strdup(yytext), yyleng);
			myUnput(s1, l1);
			advance();
			return astLVal();
		}
	}
	else
		return astPrimaryExp();
}

// FuncRParams: Exp { ',' Exp }
past astFuncRParams()
{
	int list = 1;
	past root = newAstNode();
	root->nodeType = "list";
	past func = astExp();
	past head = func;
	while (tok == ',')
	{
		list++;
		advance();
		past brother = astExp();
		func->next = brother;
		func = func->next;
	}
	root->ivalue = list;
	root->left = head;
	head->nodeType = "parameter";
	head = head->next;
	while (head != NULL)
	{
		head->nodeType = "parameter";
		head = head->next;
	}
	return root;
}

// MulExp: UnaryExp | MulExp ('*' | '/' | '%') UnaryExp
past astMulExp()
{
	past l = astUnaryExp();
	while (tok == '*' || tok == '/' || tok == '%')
	{
		int oper = tok;
		advance();
		past r = astUnaryExp();
		l = newExpr(oper, l, r, NULL);
	}
	return l;
}

// AddExp: MulExp | AddExp ('+' | '-') MulExp
past astAddExp()
{
	past l = astMulExp();
	while (tok == '+' || tok == '-')
	{
		int oper = tok;
		advance();
		past r = astMulExp();
		l = newExpr(oper, l, r, NULL);
	}
	return l;
}

// RelExp: AddExp | RelExp ('<' | '>' | '<=' | '>=') AddExp
past astRelExp()
{
	past l = astAddExp();
	while (tok == '<' || tok == '>' || tok == LESSEQ || tok == GREATEQ)
	{
		int oper = tok;
		advance();
		past r = astRelExp();
		l = newExpr(oper, l, r, NULL);
	}
	return l;
}

// EqExp: RelExp | EqExp ('==' | '!=') RelExp
past astEqExp()
{
	past l = astRelExp();
	while (tok == EQ || tok == NOTEQ)
	{
		int oper = tok;
		advance();
		past r = astRelExp();
		l = newExpr(oper, l, r, NULL);
	}
	return l;
}

// LAndExp: EqExp | LAndExp '&&' EqExp
past astLAndExp()
{
	past l = astEqExp();
	while (tok == AND)
	{
		int oper = tok;
		advance();
		past r = astEqExp();
		l = newExpr(oper, l, r, NULL);
	}
	return l;
}

// LOrExp: LAndExp | LOrExp '||' LAndExp
past astLOrExp()
{
	past l = astLAndExp();
	while (tok == OR)
	{
		int oper = tok;
		advance();
		past r = astLAndExp();
		l = newExpr(oper, l, r, NULL);
	}
	return l;
}

// ConstExp: AddExp
past astConstExp()
{
	past a = astAddExp();
	return a;
}