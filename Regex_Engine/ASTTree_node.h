#ifndef ASTTREE_NODE_H
#define ASTTREE_NODE_H

#include "global.h"

class ASTTree_node
{
    private:
        char value;                  // 值
        enum exp_kind kind;          // 属性
        ASTTree_node* left,*right;   // 左右两边节点

        friend class NFA;
    public:


        ASTTree_node(char value = 0,enum exp_kind kind = EXP_EPSILON,\
                     ASTTree_node* left = nullptr,ASTTree_node* right = nullptr)
                    :value(value),kind(kind),left(left),right(right)     {}
        ~ASTTree_node() {}

        void inorder_print_node();
        void delete_node();
};

void ASTTree_node::inorder_print_node()
{
    if(!this)   return;

    cout << "value: " << value << ",left: " << (left ? string{left->value} : "nullptr") \
         << ",right:" << (right ? string{right->value} : "nullptr");
    cout << ",prop: " ;
    switch(kind)
    {
        case EXP_EPSILON:
            cout << "EXP_EPSILON\n";
            break;
        case EXP_CHR    :
            cout << "EXP_CHR\n";
            break;
        default:
            cout << "EXP_OP\n";
    }

    left->inorder_print_node();
    right->inorder_print_node();
}

void ASTTree_node::delete_node()
{
    if(!this)   return;
    left->delete_node();
    right->delete_node();
    delete this;
}


#endif // ASTTREE_NODE_H
