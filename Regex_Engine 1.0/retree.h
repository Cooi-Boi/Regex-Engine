#ifndef __RETREE_H_
#define __RETREE_H_

#include <stack>
#include "asttree.h"
#include "nfa.h"
#include "dfa.h"


class ReTree
{
    private:
        string re;
        ASTTree ASTTree;
        NFA NFATree;
        DFA DFATree;
        vector<bool> states;
        vector<vector<int>> table;

    public:
        ReTree():re(),ASTTree(),NFATree(),DFATree(),states(),table()  {}
        ReTree(const string& str):re(str),ASTTree(re),NFATree(&ASTTree),DFATree(&NFATree),states(vector<bool> (static_cast<int>(DFATree.node_cnt),false)),\
        table(vector<vector<int>> (static_cast<int>(DFATree.node_cnt),vector<int>(256,-1)))
        {
            build();
        }

        void build();
        void table_fill(vector<vector<int>>& table,vector<bool>& states,DFA_node* root);
        void re_match(const string& str);
};

#endif // RETREE_H

