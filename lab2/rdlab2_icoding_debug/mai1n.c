#include <stdio.h>
#include <assert.h>
#include "rdlab2.h"

//为了方便调试，增加了如下两个接口，可以查看测试用例中单词的内容
// icoding 提交的代码不能使用 all_tokens 和 cur_token_num
extern TokenType all_tokens[50];
extern int cur_token_num;


// 要完成的函数
past rd_call_paras() {
    return NULL;
}


past rd_relexp() {
    // 下面这些代码只是为了说明函数的用法，与题目无关

    // 词法分析相关函数的用法
    TokenType tok1 = cur_token;
    int ind = get_cur_tok_index();
    advance();
    assert(get_cur_tok_index() == ind+1);

    TokenType tok2 = cur_token;


    advance();
    advance();

    // 回溯到之前记录的位置
    set_cur_tok_index(ind);
    TokenType same_with_tok1 = cur_token;
    assert(tok1.token == same_with_tok1.token);
    assert(get_cur_tok_index() == ind);

    //为了方便调试，增加了如下两个接口，可以查看测试用例中单词的内容
    // icoding 提交的代码不能使用 all_tokens 和 cur_token_num
    printf("当前测试用例的单词个数为： %d\n", cur_token_num);
    // 单词都保存在all_tokens数组中
    assert(tok2.token == all_tokens[1].token);
    return NULL;
}


past rd_stmt() {
    return NULL;
}
