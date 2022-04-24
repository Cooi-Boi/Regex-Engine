#ifndef __ASTTREE_H__
#define __ASTTREE_H__

#include "asttree_node.h"
using namespace std;
/*
e为终结符
EXP -> A | EXP
    -> A
A   -> B + A
A   -> B
B   -> C
    -> C*
C   -> (EXP)
    -> e
*/

class ASTTree
{
    private:
        string re;                  // 正则表达式 字符串
        ASTTree_node* root;         // Node 节点
        char token;                 // 当前处理字符
        int re_size;                // 为了get_next_token 节省反复调用re.size()函数调用的代价
        int pos;                    // 当前正在处理字符位置 get_next_token用

        friend class NFA;
    public:
        char get_next_token();
        void build_AST();
        void print_tree() const;
        void delete_node_helper();

        ASTTree():re(),root(nullptr),token(0),re_size(0),pos(-1)    {}
        ASTTree(const string& str):re(str),root(nullptr),token(0),re_size(re.size()),pos(-1)   { build_AST();}

        ~ASTTree()  {delete_node_helper();}
                                    // 根据上面BNF式子 得到以下语法转化 主要因为优先级次序
        ASTTree_node* parse_EXP();  // 处理 |
        ASTTree_node* parse_A();    // 处理 + 注意递归
        ASTTree_node* parse_B();    // 处理 * 单目运算符
        ASTTree_node* parse_C();    // 处理可能存在的
};

#endif // ASTTREE_H
