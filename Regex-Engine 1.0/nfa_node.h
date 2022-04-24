#ifndef __NFA_NODE_H__
#define __NFA_NODE_H__

#include "global.h"
#include <unordered_map>
#include <unordered_set>

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
        void Get_Id(unordered_map<NFA_node*,int>& map,unordered_set<NFA_node*>& set,NFA_node* end,int& id_max);
};

#endif // NFA_NODE_H

