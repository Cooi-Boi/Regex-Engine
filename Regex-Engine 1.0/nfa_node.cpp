#include "nfa_node.h"

inline void NFA_node::Print_NFA(NFA_node* end,string& str)
{
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

inline void NFA_node::Get_Id(unordered_map<NFA_node*,int>& map,unordered_set<NFA_node*>& set,NFA_node* end,int& id_max)
{
    if(map.find(this) != map.end() || this == end)
        return;

    set.emplace(this);
    map[this] = id_max;
    id = (id_max++);
    if(!closure_node && edge1)   edge1->Get_Id(map,set,end,id_max);
    if(edge2)                    edge2->Get_Id(map,set,end,id_max);

    return;
}
