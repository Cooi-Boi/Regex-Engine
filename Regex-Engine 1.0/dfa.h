#ifndef __DFA_H__
#define __DFA_H__

#include <deque>
#include <unordered_set>
#include "dfa_node.h"
#include "nfa.h"

class DFA
{
    private:
        DFA_node* root;
        vector<bool> visit;
        int node_cnt;
    public:
        DFA():root(nullptr),visit(),node_cnt(0) {}
        DFA(NFA* NFA):root(nullptr),visit(vector<bool> ((1 << NFA->total_nodes),false)),node_cnt(0) { build_DFA(NFA); }
        ~DFA()  {delete_all_nodes();}

        void build_DFA(NFA* NFA);
        void print_DFA_routes() const;
        void build(NFA_node* node,NFA_node* end);
        bool get_e_closure_set(unordered_set<NFA_node*>& set,deque<NFA_node*>& work_list,NFA_node* node,NFA_node* end,int& pos);

        void simplify_DFA();
        DFA_node* find_next_edge(DFA_node* node,char chr);
        void get_all_DFA_nodes(DFA_node* node,unordered_set<DFA_node*>& set,vector<DFA_node*>& total_nodes);
        void hopcraft();
        bool split(vector<vector<DFA_node*>>& sets,vector<DFA_node*>& set,unordered_map<DFA_node*,int>& map,int pos);
        void delete_all_nodes();

        friend class ReTree;
};

#endif
