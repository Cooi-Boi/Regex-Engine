#ifndef ASTTREE_H
#define ASTTREE_H

#include <vector>
#include "ASTTree_node.h"

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
        ASTTree_node* root;             // Node 节点
        string re;                      // 正则表达式 字符串
        char token;                     // 当前处理字符
        int re_size;                    // 为了get_next_token 节省反复调用re.size()函数调用的代价
        int pos;                        // 当前正在处理字符位置 get_next_token用

        friend class NFA;
    public:
        ASTTree():root(nullptr),token(0),re_size(0),pos(-1){
            re.clear();
        }

        ~ASTTree()  {delete_node_helper();}
                                    // 根据上面BNF式子 得到以下语法转化 主要因为优先级次序
        ASTTree_node* parse_EXP();  // 处理 |
        ASTTree_node* parse_A();    // 处理 + 注意递归
        ASTTree_node* parse_B();    // 处理 * 单目运算符
        ASTTree_node* parse_C();    // 处理可能存在的

        char get_next_token();
        void build_AST(string& str);
        void print_tree();

        void delete_node_helper();
};


char ASTTree::get_next_token()          // 得到下一个字符
{
    auto ret = (re_size > (++pos) ? re[pos] : 0);
    return ret;
}

void ASTTree::build_AST(string& str)    // 创建AST抽象树 递归下降处理
{
    if(str.empty())      return;

    re = str; re_size = re.size();
    pos = -1; token  = get_next_token();
    root = parse_EXP();
}

ASTTree_node* ASTTree::parse_EXP()      // 开始递归下降处理 按照最上面注释给出的 正则 BNF 来处理
{
    ASTTree_node* ret = parse_A();
    while(token == '|')
    {
        token = get_next_token();
        ASTTree_node* left = ret,*right = parse_EXP();
        ret = new ASTTree_node('|',EXP_SELECT,left,right);
    }

    return ret;
}

ASTTree_node* ASTTree::parse_A()       // 处理 +
{
    ASTTree_node* ret = parse_B();
    while(token != 0 && token != ')' && token != '|')   //可能当前为| 或者 ) 则直接不进入判断
    {
        ASTTree_node* left = ret,*right = parse_B();
        ret = new ASTTree_node('+',EXP_CONCAT,left,right);
    }
    return ret;
}

ASTTree_node* ASTTree::parse_B()       // 处理 *
{
    ASTTree_node* ret = parse_C();
    if(token == '*')
    {
        ASTTree_node* left = ret;
        ret   = new ASTTree_node(token,EXP_CLOSURE,left);
        token = get_next_token();
    }
    return ret;
}

ASTTree_node* ASTTree::parse_C()       //处理最高优先级 () 注意可能 | 是最前面的 需要特别注意
{
    ASTTree_node* ret = nullptr;
    if(token == ')')    return ret;

    if(token == '(')
    {
        token = get_next_token();
        ret = parse_EXP();
        token = get_next_token();      //跳过右边括号(
    }
    else
    {
        if(token == '|' || !token)               // 特殊情况
            ret = new ASTTree_node(0,EXP_EPSILON);
        else                                     // 正常处理 终结符
        {
            ret = new ASTTree_node(token,EXP_CHR);
            token = get_next_token();
        }
    }
    return ret;
}

void ASTTree::print_tree()              // 先序遍历 比较方便验证是否生成正确
{
    if(!this)   return;
    cout << "ASTTree_Print: " << endl;
    root->inorder_print_node();
    cout << endl;
}

void ASTTree::delete_node_helper()
{
    if(!root)   return;
    root->delete_node();
}

#endif // ASTTREE_H
