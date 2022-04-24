#include "asttree_node.h"

inline void ASTTree_node::inorder_print_node() const
{
    cout << "value: " << value << ",left: " << (left ? string{left->value} : "nullptr")
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

    if(left)    left->inorder_print_node();
    if(right)   right->inorder_print_node();
}

inline void ASTTree_node::delete_node()
{
    if(left)    left->delete_node();
    if(right)   right->delete_node();
    delete left;delete right;
}
