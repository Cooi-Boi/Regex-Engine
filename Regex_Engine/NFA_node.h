#ifndef NFA_NODE_H
#define NFA_NODE_H

#include "global.h"
#include <unordered_map>


class NFA_node
{
    private:
        char value;
        enum states state;
        NFA_node* edge1;
        NFA_node* edge2;
        int id;            // node_id For converting to DFA conveniently
        bool closure_node;  // for print_NFA_routine

        friend class NFA;
        friend class DFA_node;
        friend class DFA;
    public:
        NFA_node(char value = 0,enum states init_state = UNACCEPTABLE,    \
                 NFA_node* e1 = nullptr,NFA_node* e2 = nullptr,int id = 0,bool closure_node = false):
                 value(value),state(init_state),edge1(e1),edge2(e2),id(id),closure_node(closure_node)   {}
        ~NFA_node()         {}

        void Print_NFA(NFA_node* end,string& str);
        void Get_Id(unordered_map<NFA_node*,int>& map,NFA_node* end,int& id_max);
        void delete_node();
};



void NFA_node::Print_NFA(NFA_node* end,string& str)
{
    if(!this)   return;

    if(this == end)
    {
        cout << str << "End\n";
        return;
    }

    if(!closure_node && edge1)
    {
        str += (value == 0 ? '-' : value);
        edge1->Print_NFA(end,str);
        str.pop_back();
    }

    if(edge2)
    {
        str += (value == 0 ? '-' : value);
        edge2->Print_NFA(end,str);
        str.pop_back();
    }
}

void NFA_node::Get_Id(unordered_map<NFA_node*,int>& map,NFA_node* end,int& id_max)
{
    if(!this || map.find(this) != map.end() || this == end)
        return;

    map[this] = id_max;
    id = (id_max++);
    if(!closure_node && edge1)   edge1->Get_Id(map,end,id_max);
    if(edge2)                    edge2->Get_Id(map,end,id_max);

    return;
}

void NFA_node::delete_node()
{
    if(!this)   return;
    if(edge1 && !closure_node)   edge1->delete_node();
    if(edge2)                    edge2->delete_node();
    delete this;
}

#endif // NFA_NODE_H
