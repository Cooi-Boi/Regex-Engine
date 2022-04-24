#ifndef __NFA_H_
#define __NFA_H_

#include "NFA_node.h"

class NFA
{
    private:
        NFA_node* start;
        NFA_node* end;
        int total_nodes;
        unordered_set<NFA_node*> set;

        friend class DFA;
    public:
        NFA():start(new NFA_node()),end(new NFA_node(0,ACCEPTABLE)),total_nodes(0),set{}
        {
            build_NFA(nullptr);
        }

        NFA(ASTTree* Tree):start(new NFA_node()),end(new NFA_node(0,ACCEPTABLE)),total_nodes(0),set{}
        {
            build_NFA(Tree);
        }

        ~NFA()
        {
            if(!start)  return;
            for(auto& node:set)
                delete node;
        }

        void build_NFA(ASTTree* Tree);
        void build(NFA_node* start,NFA_node* end,ASTTree_node* Tree);
        void epsilon_char_build(NFA_node* start,NFA_node* end,ASTTree_node* node);
        void concat_build(NFA_node* start,NFA_node* end,ASTTree_node* node);
        void select_build(NFA_node* start,NFA_node* end,ASTTree_node* node);
        void closure_build(NFA_node* start,NFA_node* end,ASTTree_node* node);
        void print_NFA_routes() const;
        void get_all_nodes_id();
};

#endif // NFA_H

