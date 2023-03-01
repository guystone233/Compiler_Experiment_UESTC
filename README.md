# Compiler_Experiment_UESTC
电子科技大学信息与软件工程学院编译技术课程实验代码
## lab1
Icoding题干：   
请根据词法分析实验中给定的文法，利用flex设计一词法分析器，该分析器从标准输入读入源代码后，输出单词的类别编号及附加信息。  
附加信息规定如下：   
当类别为 Y_ID、num_INT 或 num_FLOAT 时，附加信息为该类别对应的属性，如 main, 100, 29.3等；   
当类别为 关键字 时，附件信息为 KEYWORD； 当类别为 运算符 时，附件信息为 OPERATOR；   
当类别为 其它符号时，附件信息为 SYMBOL；   
例如对于源代码  
```C
int main(){
    return 3;
}
```
词法分析器的输出为：  
```HTML
<261, KEYWORD>
<260, main>
<285, SYMBOL>
<286, SYMBOL>
<287, SYMBOL>
<269, KEYWORD>
<258, 3>
<292, SYMBOL>
<288, SYMBOL>
```
请在本页面上传词法分析实验所有源代码及实验报告（源代码至少包括一个flex源文件，即 .l 文件，如有其它 .c 或 .h 文件，也请一并上传）  
本实验中的单词类别定义如下，该定义包含在 token.h 中，此头文件可直接使用，无需上传  
## lab2
Icoding题干：
请在本页面最下方上传实验所有源代码及实验报告（这里的源代码是指已经完成，但没有通过下面在线测试的代码）  
再根据实验所给文法，完成非终结符 CallParams、RelExp、Stmt对应的递归下降函数。  
```C
past rd_relexp();
past rd_call_paras();
past rd_stmt();
```
其中非终结符 Block 和 ArraySubscripts 对应的递归下降函数已实现，可直接调用，函数的声明如下：  
```C
past rd_block();
past rd_array_subscripts();
```
词法分析的设定如下：  
```C
// 单词类别
enum yytokentype {
	num_INT = 258,
	num_FLOAT = 259,
	Y_ID = 260,
	...
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
```
抽象语法树的设定如下：  
```C
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
```
## lab3
Icoding题干：
请根据LR语法分析实验中给定的文法，利用bison设计一语法分析器，该分析器从标准输入读入源代码后，得到该源码对应的抽象语法树。  
请在本页面上传本实验所有源代码及实验报告（源代码至少包括一个flex源文件，即 .l 文件，一个 bison 源文件，即 .y 文件，如有其它 .c 或 .h 文件，也请一并上传）  
本实验中的单词类别定义如下，该定义包含在 token.h 中，此头文件可直接使用，无需上传  
本实验仅上传所需的文件，并确保所提交文件能正确编译即可，系统不对代码的正确性进行检查。  
```C
// 单词类别
enum yytokentype {
   num_INT = 258,
   num_FLOAT = 259,

   Y_ID = 260,

   Y_INT = 261,
   ...
};
```
抽象语法树的设定如下：  
```C
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
```
部分代码Credit:https://gitee.com/Brett-Tseng/Syntax-Analyse-Bison
## lab4
实验任务：
1.查看指令手册：LLVMRef.mht  
2.clang学习：编写简单的 C语言程序 test.c，用 clang -emit-llvm -S ./test.c 生成该文件对应的llvm指令  
3.学习示例程序，理解代码生成过程  
4.在实验3及示例程序的基础上完成算术表达式、逻辑表达式赋值语句、条件语句、循环语句对应的代码生成，函数命名分别为：  
```C
genArithmeticExpr, genLogicExpr
genAssignStmt, genIfStmt, genWhileStmt
```
 函数参数都为： ``(past node, char* result)``  
         node为相关类型的结点，  
         result 为用来保存三地址代码/LLVM虚拟指令的存储空间  
         每行只放一条指令  
LR分析器部分代码Credit:https://gitee.com/Brett-Tseng/Syntax-Analyse-Bison  
