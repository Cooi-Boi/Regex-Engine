#include "nfa.h"

inline void NFA::print_NFA_routes() const
{
    string str;
    cout << "NFA_Print: \n";
    start->Print_NFA(end,str);
    cout << endl;
}

inline void NFA::build_NFA(ASTTree* Tree)      // 涉及内存泄露 后面再改
{
    if(!Tree)   return;

    start->edge1 = end;
    build(start,end,Tree->root);
    get_all_nodes_id();
}

inline void NFA::build(NFA_node* start,NFA_node* end,ASTTree_node* node)
{
    if(!node)   return;
    switch(node->kind)
    {
        case EXP_CHR:
        case EXP_EPSILON:
            epsilon_char_build(start,end,node);
            break;
        case EXP_CONCAT:
            concat_build(start,end,node);
            break;
        case EXP_SELECT:
            select_build(start,end,node);
            break;
        case EXP_CLOSURE:
            closure_build(start,end,node);
            break;
        default:
            cout << "NFA_Tree build Error,Unidentified Char\n";
            exit(0);
    }
    return;
}

inline void NFA::epsilon_char_build(NFA_node* start,NFA_node* end,ASTTree_node* node)
{
    start->value = node->value;
}

inline void NFA::concat_build(NFA_node* start,NFA_node* end,ASTTree_node* node)
{
    NFA_node* n1 = new NFA_node(),*n2 = new NFA_node();

    start->edge1 = n1;
    n1->edge1 = n2;
    n2->edge1 = end;

    build(start,n1,node->left);
    build(n2,end,node->right);
}

inline void NFA::select_build(NFA_node* start,NFA_node* end,ASTTree_node* node)
{
    NFA_node* n1 = new NFA_node(),*n2 = new NFA_node();
    NFA_node* n1_next = new NFA_node(),*n2_next = new NFA_node();

    start->edge1 = n1;
    start->edge2 = n2;
    n1->edge1 = n1_next;
    n2->edge1 = n2_next;
    n1_next->edge1 = end;
    n2_next->edge1 = end;

    build(n1,n1_next,node->left);
    build(n2,n2_next,node->right);
}

inline void NFA::closure_build(NFA_node* start,NFA_node* end,ASTTree_node* node)
{
    NFA_node* n1 = new NFA_node(),*n2 = new NFA_node();

    start->edge1 = n1;
    start->edge2 = end;
    n1->edge1 = n2;
    n2->edge1 = n1;
    n2->edge2 = end;
    n2->closure_node = true;

    build(n1,n2,node->left);
}

inline void NFA::get_all_nodes_id()
{
    unordered_map<NFA_node*,int> map;
    int id_max = 0;
    start->Get_Id(map,set,end,id_max);
    end->id = (id_max++);
    total_nodes = id_max;
}
