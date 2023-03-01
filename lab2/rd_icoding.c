#include "node_type.h"

/*enum yytokentype
{
    num_INT = 258,
    num_FLOAT = 259,

    Y_ID = 260,

    Y_INT = 261,
    Y_VOID = 262,
    Y_CONST = 263,
    Y_IF = 264,
    Y_ELSE = 265,
    Y_WHILE = 266,
    Y_BREAK = 267,
    Y_CONTINUE = 268,
    Y_RETURN = 269,

    Y_ADD = 270,
    Y_SUB = 271,
    Y_MUL = 272,
    Y_DIV = 273,
    Y_MODULO = 274,
    Y_LESS = 275,
    Y_LESSEQ = 276,
    Y_GREAT = 277,
    Y_GREATEQ = 278,
    Y_NOTEQ = 279,
    Y_EQ = 280,
    Y_NOT = 281,
    Y_AND = 282,
    Y_OR = 283,
    Y_ASSIGN = 284,

    Y_LPAR = 285,
    Y_RPAR = 286,
    Y_LBRACKET = 287,
    Y_RBRACKET = 288,
    Y_LSQUARE = 289,
    Y_RSQUARE = 290,
    Y_COMMA = 291,
    Y_SEMICOLON = 292,

    Y_FLOAT = 293
};
typedef struct _TokenType{
	enum yytokentype token; // 单词类别
	union {
		int		ivalue;
		float   fvalue;
		char*	svalue;
	}attr; // 单词属性
}TokenType;

extern TokenType cur_token;//全局变量，表示当前的单词
TokenType advance(); //读取下一个单词，将其保存于 cur_token 中，同时返回该单词

// 下面两个函数用于可能的回溯处理，即如果有多个分支需要尝试，在尝试语法分析前，可调用get_cur_tok_index获取当前单词的位置，
// 之后在分析时可能会调用若干次advance函数，如果分析失败，则可通过set_cur_tok_index函数重置当前单词的位置。
int get_cur_tok_index(); //所有单词已经读入单词数组中，该函数获取当前单词在数组中的下标
void set_cur_tok_index(int ind); //将 cur_token 设置为下标指定的token值

typedef struct _ast ast;
typedef struct _ast *past;

struct _ast{
   int ivalue;
   float fvalue;
   char* svalue;
   node_type nodeType;
   past left;
   past right;
   past if_cond;
   past next;
};

past newAstNode();// 创建一个结点
past newID(char* value); //创建一个结点，并将结点中的 svalue 设置为 value
past newInt(int value); //创建一个结点，并将结点中的 ivalue 设置为 value
*/
/*Exp         → AddExp
PrimaryExp  → '(' Exp ')' | LVal | num_INT | num_FLOAT
UnaryExp    → PrimaryExp | id '(' CallParams ')' | '+' UnaryExp | '-' UnaryExp | '!' UnaryExp
CallParams  → Exp CallParams1 | ε
CallParams1 → ',' Exp CallParams1 | ε
MulExp      → UnaryExp MulExp1
MulExp1  → '*' UnaryExp MulExp1 | '/' UnaryExp MulExp1 | '%' UnaryExp MulExp1 | ε
AddExp      → MulExp AddExp1
AddExp1  → '+' MulExp AddExp1 | '-' MulExp AddExp1 | ε
RelExp      → AddExp RelExp1
RelExp1  → '<' AddExp RelExp1 | '>' AddExp RelExp1 | '<=' AddExp RelExp1 | '>=' AddExp RelExp1 | ε
LVal → id LVal1
LVal1 → ArraySubscripts| ε
ArraySubscripts → [ Exp ] ArraySubscripts1
ArraySubscripts1 → [ Exp ] ArraySubscripts1 | ε
Stmt→ LVal Y_ASSIGN Exp Y_SEMICOLON | Exp Y_SEMICOLON
*/

/*  Stmt: LVal Y_ASSIGN Exp Y_SEMICOLON
          | Y_SEMICOLON
          | Exp Y_SEMICOLON
          | Block
          | Y_WHILE Y_LPAR LOrExp Y_RPAR Stmt
          | Y_IF Y_LPAR LOrExp Y_RPAR Stmt
          | Y_IF Y_LPAR LOrExp Y_RPAR Stmt Y_ELSE Stmt
          | Y_BREAK Y_SEMICOLON
          | Y_CONTINUE Y_SEMICOLON
          | Y_RETURN Exp Y_SEMICOLON
          | Y_RETURN Y_SEMICOLON
    EqExp: RelExp
           | RelExp Y_EQ EqExp
           | RelExp Y_NOTEQ EqExp

      LAndExp: EqExp
             | EqExp Y_AND LAndExp

      LOrExp: LAndExp
            | LAndExp Y_OR LOrExp
          
          */
#include "rdlab2.h"
past rd_relexp();
past rd_relexp1();
past rd_exp();
past rd_addexp();
past rd_addexp1();
past rd_mulexp();
past rd_mulexp1();
past rd_unaryexp();
past rd_primaryexp();
past rd_call_paras();
past rd_call_paras1();
past rd_eqexp();
past rd_eqexp1();
past rd_landexp();
past rd_landexp1();
past rd_lorexp();
past rd_lorexp1();
past rd_stmtelse();



past rd_call_paras(){
    past l=rd_exp();
    past r=rd_call_paras1();
    if(r!=NULL){
        l->next=r;
        return l;
    }else return l;
}
past rd_call_paras1(){
    if(cur_token.token == Y_COMMA){
        advance();
        past l=rd_exp();
        past r=rd_call_paras1();
        if(r!=NULL){
            l->next=r;
            return l;
        }else return l;
    }else return NULL;
}

past rd_relexp(){
    past l=rd_addexp();
    past r=rd_relexp1();
    if(r!=NULL){
        r->left=l;
        return r;
    }else return l;
}
past rd_relexp1(){
    if(cur_token.token == Y_LESSEQ){
        advance();
        past l=rd_addexp();
        past r=rd_relexp1();
        if(r!=NULL){
            r->left=l;
            past p=newAstNode();
            p->right = r;
            p->nodeType=BINARY_OPERATOR;
            p->ivalue=Y_LESSEQ;
            return p;
        }else{
            past p=newAstNode();
            p->nodeType=BINARY_OPERATOR;
            p->ivalue=Y_LESSEQ;
            p->right=l;
            return p;
        }
    }else if(cur_token.token == Y_GREATEQ){
        advance();
        past l=rd_addexp();
        past r=rd_relexp1();
        if(r!=NULL){
            r->left=l;
            past p=newAstNode();
            p->right = r;
            p->nodeType=BINARY_OPERATOR;
            p->ivalue=Y_GREATEQ;
            return p;
        }else{
            past p=newAstNode();
            p->nodeType=BINARY_OPERATOR;
            p->ivalue=Y_GREATEQ;
            p->right=l;
            return p;
        }
    }else if(cur_token.token == Y_LESS){
        advance();
        past l=rd_addexp();
        past r=rd_relexp1();
        if(r!=NULL){
            r->left=l;
            past p=newAstNode();
            p->right = r;
            p->nodeType=BINARY_OPERATOR;
            p->ivalue=Y_LESS;
            return p;
        }else{
            past p=newAstNode();
            p->nodeType=BINARY_OPERATOR;
            p->ivalue=Y_LESS;
            p->right=l;
            return p;
        }
    }else if(cur_token.token == Y_GREAT){
        advance();
        past l=rd_addexp();
        past r=rd_relexp1();
        if(r!=NULL){
            r->left=l;
            past p=newAstNode();
            p->right = r;
            p->nodeType=BINARY_OPERATOR;
            p->ivalue=Y_GREAT;
            return p;
        }else{
            past p=newAstNode();
            p->nodeType=BINARY_OPERATOR;
            p->ivalue=Y_GREAT;
            p->right=l;
            return p;
        }
    }else return NULL;
}


past rd_exp()
{
    return rd_addexp();
}
past rd_addexp(){
    past l=rd_mulexp();
    past r=rd_addexp1();
    if(r!=NULL){
        r->left=l;
        return r;
    }else return l;

}

past rd_addexp1()
{
    if(cur_token.token == Y_ADD)
    {
        advance();
        past l = rd_mulexp();
        past r = rd_addexp1();
        if(r!=NULL){
            r -> left = l;
            past p = newAstNode();
            p->right = r;
            p->nodeType = BINARY_OPERATOR;
            p->ivalue = Y_ADD;
            return p;
        }else{
            past p = newAstNode();
            p->nodeType = BINARY_OPERATOR;
            p->ivalue = Y_ADD;
            p->right = l;
            return p;
        }
    }
    else if(cur_token.token == Y_SUB)
    {
        advance();
        past l = rd_mulexp();
        past r = rd_addexp1();
        if(r!=NULL){
            r -> left = l;
            past p = newAstNode();
            p->right = r;
            p->nodeType = BINARY_OPERATOR;
            p->ivalue = Y_SUB;
            return p;
        }else{
            past p = newAstNode();
            p->nodeType = BINARY_OPERATOR;
            p->ivalue = Y_SUB;
            p->right = l;
            return p;
        }
    }
    else
    {
        return NULL;
    }
}

past rd_mulexp()
{
    past l=rd_unaryexp();
    past r=rd_mulexp1();
    if(r!=NULL){
        r->left=l;
        return r;
    }else return l;
}

past rd_mulexp1()
{
    if(cur_token.token == Y_MUL)
    {
        advance();
        past l = rd_unaryexp();
        past r = rd_mulexp1();
        if(r!=NULL){
            r -> left = l;
            past p = newAstNode();
            p->right = r;
            p->nodeType = BINARY_OPERATOR;
            p->ivalue = Y_MUL;
            return p;
        }else{
            past p = newAstNode();
            p->nodeType = BINARY_OPERATOR;
            p->ivalue = Y_MUL;
            p->right = l;
            return p;
        }
    }
    else if(cur_token.token == Y_DIV)
    {
        advance();
        past l = rd_unaryexp();
        past r = rd_mulexp1();
        if(r!=NULL){
            r -> left = l;
            past p = newAstNode();
            p->right = r;
            p->nodeType = BINARY_OPERATOR;
            p->ivalue = Y_DIV;
            return p;
        }else{
            past p = newAstNode();
            p->nodeType = BINARY_OPERATOR;
            p->ivalue = Y_DIV;
            p->right = l;
            return p;
        }
    }
    else if(cur_token.token == Y_MODULO)
    {
        advance();
        past l = rd_unaryexp();
        past r = rd_mulexp1();
        if(r!=NULL){
            r -> left = l;
            past p = newAstNode();
            p->right = r;
            p->nodeType = BINARY_OPERATOR;
            p->ivalue = Y_MODULO;
            return p;
        }else{
            past p = newAstNode();
            p->nodeType = BINARY_OPERATOR;
            p->ivalue = Y_MODULO;
            p->right = l;
            return p;
        }
    }
    else
    {
        return NULL;
    }
}
past rd_unaryexp(){
    if(cur_token.token == Y_ADD)
    {
        advance();
        past l = rd_unaryexp();
        past p = newAstNode();
        p->nodeType = UNARY_OPERATOR;
        p->ivalue = Y_ADD;
        p->right = l;
        return p;
    }
    else if(cur_token.token == Y_SUB)
    {
        advance();
        past l = rd_unaryexp();
        past p = newAstNode();
        p->nodeType = UNARY_OPERATOR;
        p->ivalue = Y_SUB;
        p->right = l;
        return p;
    }
    else if(cur_token.token == Y_NOT)
    {
        advance();
        past l = rd_unaryexp();
        past p = newAstNode();
        p->nodeType = UNARY_OPERATOR;
        p->ivalue = Y_NOT;
        p->right = l;
        return p;
    }
    else
    {
        return rd_primaryexp();
    }
}
past rd_primaryexp(){
    if(cur_token.token == Y_LPAR)
    {
        advance();
        past l = rd_exp();
        if(cur_token.token == Y_RPAR)
        {
            advance();
            return l;
        }
    }else if(cur_token.token == num_INT)
    {
        past p = newInt(cur_token.attr.ivalue);

        advance();
        return p;
    }else if(cur_token.token == Y_ID){
        past node = newAstNode();
        //node->nodeType = DECL_REF_EXPR;
        node->svalue = cur_token.attr.svalue;
        past l = node;
        advance();
        if(cur_token.token == Y_LSQUARE)return rd_array_subscripts();
        else return l;
    }
    return NULL;
        
}
past rd_eqexp(){
    past l = rd_relexp();
    past r = rd_eqexp1();
    if(r!=NULL){
        r->left=l;
        return r;
    }else return l;
}
past rd_eqexp1(){
    if(cur_token.token == Y_EQ){
        advance();
        past l = rd_relexp();
        past r = rd_eqexp1();
        if(r!=NULL){
            r -> left = l;
            past p = newAstNode();
            p->right = r;
            p->nodeType = BINARY_OPERATOR;
            p->ivalue = Y_EQ;
            return p;
        }else{
            past p = newAstNode();
            p->nodeType = BINARY_OPERATOR;
            p->ivalue = Y_EQ;
            p->right = l;
            return p;
        }
    }else if(cur_token.token == Y_NOTEQ){
        advance();
        past l = rd_relexp();
        past r = rd_eqexp1();
        if(r!=NULL){
            r -> left = l;
            past p = newAstNode();
            p->right = r;
            p->nodeType = BINARY_OPERATOR;
            p->ivalue = Y_NOTEQ;
            return p;
        }else{
            past p = newAstNode();
            p->nodeType = BINARY_OPERATOR;
            p->ivalue = Y_NOTEQ;
            p->right = l;
            return p;
        }
    }else{
        return NULL;
    }
}
past rd_landexp(){
    past l = rd_eqexp();
    past r = rd_landexp1();
    if(r!=NULL){
        r->left=l;
        return r;
    }else return l;
}
past rd_landexp1(){
    if(cur_token.token == Y_AND){
        advance();
        past l = rd_eqexp();
        past r = rd_landexp1();
        if(r!=NULL){
            r -> left = l;
            past p = newAstNode();
            p->right = r;
            p->nodeType = BINARY_OPERATOR;
            p->ivalue = Y_AND;
            return p;
        }else{
            past p = newAstNode();
            p->nodeType = BINARY_OPERATOR;
            p->ivalue = Y_AND;
            p->right = l;
            return p;
        }
    }else{
        return NULL;
    }
}
past rd_lorexp(){
    past l = rd_landexp();
    past r = rd_lorexp1();
    if(r!=NULL){
        r->left=l;
        return r;
    }else return l;
}
past rd_lorexp1(){
    if(cur_token.token == Y_OR){
        advance();
        past l = rd_landexp();
        past r = rd_lorexp1();
        if(r!=NULL){
            r -> left = l;
            past p = newAstNode();
            p->right = r;
            p->nodeType = BINARY_OPERATOR;
            p->ivalue = Y_OR;
            return p;
        }else{
            past p = newAstNode();
            p->nodeType = BINARY_OPERATOR;
            p->ivalue = Y_OR;
            p->right = l;
            return p;
        }
    }else{
        return NULL;
    }
}
past rd_stmtelse(){
    if(cur_token.token == Y_ELSE){
        advance();
        return rd_stmt();
    }else{
        return rd_stmt();
    }
}
past rd_stmt(){
    if(cur_token.token == Y_LBRACKET){
        return rd_block();
    }else if(cur_token.token == Y_SEMICOLON){
        advance();
        return NULL;
    }else if(cur_token.token == Y_IF){
        past p = newAstNode();
        p->nodeType = IF_STMT;
        advance();
        if(cur_token.token == Y_LPAR){
            advance();
            past l = rd_lorexp();
            if(cur_token.token == Y_RPAR){
                advance();
                past r = rd_stmt();
                p->if_cond = l;
                p->left = r;
                past q = rd_stmtelse();
                if(q!=NULL){
                    p->right = q;
                }
                return p;
            }
        }
    }else if(cur_token.token == Y_BREAK){
        past p = newAstNode();
        p->nodeType = BREAK_STMT;
        advance();
        if(cur_token.token == Y_SEMICOLON){
            advance();
            return p;
        }
    }else if(cur_token.token == Y_CONTINUE){
        past p = newAstNode();
        p->nodeType = CONTINUE_STMT;
        advance();
        if(cur_token.token == Y_SEMICOLON){
            advance();
            return p;
        }
    }else if(cur_token.token == Y_RETURN){
        past p = newAstNode();
        p->nodeType = RETURN_STMT;
        advance();
        if(cur_token.token == Y_SEMICOLON){
            advance();
            return p;
        }else{
            past l = rd_exp();
            if(l!=NULL){
                p->left = l;
                if(cur_token.token == Y_SEMICOLON){
                    advance();
                    return p;
                }
            }
        }
    }
    else if(cur_token.token == Y_ID){
        past node = newAstNode();
        node->nodeType = DECL_REF_EXPR;
        node->svalue = cur_token.attr.svalue;
        advance();
        if(cur_token.token == Y_LSQUARE)node = rd_array_subscripts();
        if(cur_token.token == Y_ASSIGN){
            advance();
            past r = rd_exp();
            if(cur_token.token == Y_SEMICOLON){
                advance();
                past p = newAstNode();
                p->nodeType = BINARY_OPERATOR;
                p->ivalue = '=';
                p->left = node;
                p->right = r;
                return p;
            }
        }
    }else if(cur_token.token == Y_WHILE){
        past p = newAstNode();
        p->nodeType = WHILE_STMT;
        advance();
        if(cur_token.token == Y_LPAR){
            advance();
            past l = rd_lorexp();
            if(cur_token.token == Y_RPAR){
                advance();
                past r = rd_stmt();
                p->left = l;
                p->right = r;
                return p;
            }
        }
    }
    return NULL;
    
}
