#ifndef RETREE_H
#define RETREE_H

#include <stack>
#include "ASTTree.h"
#include "NFA.h"
#include "DFA.h"


class ReTree
{
    private:
        string re{};
        ASTTree ASTTree{};
        NFA NFATree{};
        DFA DFATree{};
        vector<bool> states{};
        vector<vector<int>> table{};

    public:
        ReTree()    {}

        ReTree(string& str)     {re = str;}

        void build(string& str);
        void table_fill(vector<vector<int>>& table,vector<bool>& states,DFA_node* root);
        void re_match(string& str);
};

void ReTree::table_fill(vector<vector<int>>& table,vector<bool>& states,DFA_node* root)
{
    if(!root)   return;

    unordered_set<DFA_node*> set;
    deque<DFA_node*> work_list{root};

    while(!work_list.empty())
    {
        auto now_node = work_list.front();
        work_list.pop_front();

        auto id = now_node->id;
        states[id] = now_node->state;
        int size = now_node->next_values.size();
        for(int i=0;i<size;++i)
        {
            table[id][now_node->next_values[i]] = now_node->next_work_edges[i]->id;
            if(set.find(now_node->next_work_edges[i]) == set.end())
            {
                set.emplace(now_node->next_work_edges[i]);
                work_list.emplace_back(now_node->next_work_edges[i]);
            }
        }
    }

    return;
}

void ReTree::build(string& str)
{
    re = str;
    ASTTree.build_AST(re);
    //ASTTree.print_tree();

    NFATree.build_NFA(&ASTTree);
    //NFATree.print_NFA_routes();

    DFATree.build_DFA(&NFATree);
    //DFATree.print_DFA_routes();
    DFATree.simplify_DFA();
    //DFATree.print_DFA_routes();

    int size = DFATree.node_cnt;
    table = vector<vector<int>> (size,vector<int>(256,-1));
    states = vector<bool> (size,false);

    table_fill(table,states,DFATree.root);
}

void ReTree::re_match(string& str)
{
    cout << endl;
    int pos = 0,size = str.size(),state = states[pos],times = 0,startpos = 0,output_endpos = -1;
    string matched_str{},output_str;
    stack<int> pos_stack,matched_size_stack;
    if(state)   pos_stack.emplace(pos);

    for(int i=0;i<=size;)
    {
        if(i == size)   pos = -1;
        if(pos == -1)
        {
            while(!pos_stack.empty())
            {
                pos = pos_stack.top();
                pos_stack.pop();

                state = pos != -1 ? states[pos] : false;
                if(state == true)
                {
                    if(matched_size_stack.empty())  output_str = matched_str;
                    matched_size_stack.emplace(matched_str.size());
                }

                if(!matched_str.empty())
                    matched_str.pop_back();
            }

            int inc_size = (!matched_size_stack.empty() ? matched_size_stack.top() : 1);
            int end_pos = startpos + output_str.size();
            inc_size = max(inc_size,1);

            while(!matched_size_stack.empty())
            {
                if(matched_size_stack.size() == 1 && end_pos > output_endpos)
                {
                    cout << "[Love 6] Match " << right << setw(2) << (++times) ;
                    cout << "|" << left << setw(6) <<  (to_string(startpos) + "-" + to_string(startpos + output_str.size()));
                    output_endpos = max(output_endpos,end_pos);
                    cout << "|" << output_str << " " << endl;
                }
                matched_size_stack.pop();
            }

            startpos += inc_size;
            i = startpos;

            pos = 0;state = states[pos];
            if(state == true)
                pos_stack.emplace(pos);
        }
        else
        {
            auto chr = str[i];
            pos = table[pos][chr];
            matched_str += chr;
            pos_stack.emplace(pos);
            ++i;
        }
    }

    if(!times)
        cout << "Your regular expression does not match the subject string.\n";
    cout << "------------------------------------------------------------------------\n";
}

#endif // RETREE_H
