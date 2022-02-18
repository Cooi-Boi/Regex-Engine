#ifndef __DFA_NODE_H__
#define __DFA_NODE_H__

#include <vector>
#include "global.h"

class DFA_node
{
    private:
        vector<char>      next_values;
        vector<DFA_node*> next_work_edges;

        int id;
        bool state;
        friend class DFA;
        friend class ReTree;

    public:
        DFA_node(int n,bool init_state = UNACCEPTABLE):next_values(),next_work_edges(),id(n),state(init_state){}

        void print_DFA(unordered_set<DFA_node*>& set);
};

#endif // DFA_NODE_H
