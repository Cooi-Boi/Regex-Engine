#include "asttree.h"

inline void ASTTree::build_AST()    // 创建AST抽象树 递归下降处理
{
    if(re.empty())      return;

    token  = get_next_token();
    root = parse_EXP();
}

inline void ASTTree::delete_node_helper()
{
    if(!root)   return;
    root->delete_node();
    delete root;
}

inline char ASTTree::get_next_token()          // 得到下一个字符
{
    return (re_size > (++pos) ? re[pos] : 0);
}

inline void ASTTree::print_tree() const          // 中序遍历 比较方便验证是否生成正确
{
    if(!root)   return;
    cout << "ASTTree_Print: " << endl;
    root->inorder_print_node();
    cout << endl;
}

inline ASTTree_node* ASTTree::parse_EXP()      // 开始递归下降处理 按照最上面注释给出的 正则 BNF 来处理
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

inline ASTTree_node* ASTTree::parse_A()       // 处理 +
{
    ASTTree_node* ret = parse_B();
    while(token != 0 && token != ')' && token != '|')       //可能当前为| 或者 ) 则直接不进入判断
    {
        ASTTree_node* left = ret,*right = parse_B();
        ret = new ASTTree_node('+',EXP_CONCAT,left,right);
    }
    return ret;
}

inline ASTTree_node* ASTTree::parse_B()       // 处理 *
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

inline ASTTree_node* ASTTree::parse_C()       //处理最高优先级 () 注意可能 | 是最前面的 需要特别注意
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
        else                           // 正常处理 终结符
        {
            ret = new ASTTree_node(token,EXP_CHR);
            token = get_next_token();
        }
    }
    return ret;
}
