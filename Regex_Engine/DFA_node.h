#ifndef DFA_NODE_H
#define DFA_NODE_H

#include <vector>
#include "global.h"

class DFA_node
{
    private:
        vector<char>      next_values{};
        vector<DFA_node*> next_work_edges{};

        int id;
        bool state;
        friend class DFA;
        friend class ReTree;

    public:
        DFA_node(int n,bool init_state = UNACCEPTABLE)
        {
            id = n;
            state = init_state;
        }

        void print_DFA(unordered_set<DFA_node*>& set);
};

void DFA_node::print_DFA(unordered_set<DFA_node*>& set)
{
    if(!this || set.find(this) != set.end())   return;
    cout << "now_DFA_id: " << id;

    //注释部分是 判断是否正确划分到一个子集中
    /*cout << ",now_NFA_ids: ";
    for(const auto& node: work_list)
        cout << node->id << " ";*/

    cout << ",state: " << (state == ACCEPTABLE ? "ACCEPTABLE" : "UNACCEPTABLE") << endl;
    set.emplace(this);

    int size = next_work_edges.size();
    cout << "next_DFA_id & cost_chr: ";
    for(int i=0;i<size;++i)
        cout << next_values[i] << "->" << next_work_edges[i]->id << " ";

    cout << "\n" << endl;
    for(int i=0;i<size;++i)
        next_work_edges[i]->print_DFA(set);
}

#endif // DFA_NODE_H
