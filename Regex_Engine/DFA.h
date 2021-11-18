#ifndef DFA_H
#define DFA_H

#include <deque>
#include <unordered_set>
#include "DFA_node.h"

class DFA
{
    private:
        DFA_node* root;
        vector<bool> visit{};
        int node_cnt;
    public:
        DFA():root(nullptr),node_cnt(0) {}
        ~DFA()  {delete_all_nodes();}

        void build_DFA(NFA* NFA);
        void print_DFA_routes();
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

void DFA::print_DFA_routes()
{
    unordered_set<DFA_node*> set;
    cout << "DFA_Print: " <<endl;
    cout << "Node_cnt: " << node_cnt << endl;
    root->print_DFA(set);
}

void DFA::build_DFA(NFA* NFA)
{
    visit = vector<bool> ((1 << NFA->total_nodes),false);
    build(NFA->start,NFA->end);
}

void DFA::build(NFA_node* start,NFA_node* end)
{
    deque<DFA_node*> work_lists{};
    deque<NFA_node*> work_list{},tmp{};
    deque<deque<NFA_node*>> next_work_lists{};

    unordered_set<NFA_node*> set;
    unordered_map<int,DFA_node*> cnt;

    int pos = 0,nodes = 0,id_max = 0;
    bool state = get_e_closure_set(set,tmp,start,end,pos);

    if(!tmp.empty())
    {
        auto node = new DFA_node(id_max++,state);
        work_lists.emplace_back(node);
        next_work_lists.emplace_back(tmp);
        root = node;
        visit[pos] = true;
        cnt[pos]   = node;
        ++nodes;
    }

    while(!work_lists.empty())
    {
        auto now_node = work_lists.front();
        work_lists.pop_front();

        work_list = next_work_lists.front();
        next_work_lists.pop_front();

        tmp.clear();
        set.clear();

        for(int i=1;i<256;++i)          // epsilon的情况不考虑进去 因为此时加入epsilon 全为此时工作集的
        {
            pos = 0;
            state = false;

            for(const auto& ptr:work_list)
            {
                if(ptr->value != i) continue;

                if(ptr->edge1)      state |= get_e_closure_set(set,tmp,ptr->edge1,end,pos);
                if(ptr->edge2)      state |= get_e_closure_set(set,tmp,ptr->edge2,end,pos);
            }

            if(!tmp.empty())
            {
                DFA_node* node = nullptr;
                if(visit[pos])          node = cnt[pos];
                else
                {
                    node = new DFA_node(id_max++,state);
                    visit[pos] = true;
                    cnt[pos] = node;
                    work_lists.emplace_back(node);
                    next_work_lists.emplace_back(tmp);
                    ++nodes;
                }

                now_node->next_values.emplace_back(static_cast<char>(i));
                now_node->next_work_edges.emplace_back(node);
                tmp.erase(tmp.begin(),tmp.end());
                set.clear();
            }
        }
    }
    node_cnt = id_max;
}

bool DFA::get_e_closure_set(unordered_set<NFA_node*>& set,deque<NFA_node*>& work_list,NFA_node* node,NFA_node* end,int& pos)
{
    if(!node)   return false;
    bool acceptable_state = false;
    deque<NFA_node*> tmp{node};

    while(!tmp.empty())
    {
        auto ptr = tmp.front();
        tmp.pop_front();
        if(set.find(ptr) != set.end())    continue;

        if(!ptr->value)
        {
            if(ptr->edge1)     tmp.emplace_back(ptr->edge1);
            if(ptr->edge2)     tmp.emplace_back(ptr->edge2);
        }

        work_list.emplace_back(ptr);
        pos |= (1 << ptr->id);
        set.emplace(ptr);

        if(ptr == end)
            acceptable_state = true;
    }
    return acceptable_state;
}

void DFA::simplify_DFA()
{
    hopcraft();
}

DFA_node* DFA::find_next_edge(DFA_node* node,char chr)
{
    if(node->next_values.empty())   return nullptr;

    int size = node->next_values.size();
    for(int i=0;i<size;++i)
    {
        auto tmp = node->next_values[i];
        if(tmp == chr)
            return node->next_work_edges[i];
    }
    return nullptr;
}

bool DFA::split(vector<vector<DFA_node*>>& sets,vector<DFA_node*>& set,unordered_map<DFA_node*,int>& map,int pos)
{
    if(set.size() <= 1)   return false;

    unordered_map<int,vector<DFA_node*>> DFA_map;
    unordered_set<int> id_set;
    int id_first = 0,new_id = -256;


    for(int i=1;i<256;++i)
    {
        auto chr = static_cast<char>(i);
        for(const auto& node:set)
        {
            auto next_ptr = find_next_edge(node,chr);
            int tmp_pos = (next_ptr ? map[next_ptr] : 0);

            if(!next_ptr)             //因为没有边链接 所以属于单独的状态
            {
                tmp_pos = new_id;
                map[node] = new_id;
                ++new_id;
            }
            id_set.emplace(tmp_pos);
        }

        if(id_set.size() != 1)
        {
            id_set.clear();
            for(const auto& node:set)
            {
                auto next_ptr = find_next_edge(node,chr);
                int pos = (next_ptr ? map[next_ptr] : map[node]);
                id_set.emplace(pos);

                DFA_map[pos].emplace_back(node);
            }

            for(const auto& num:id_set)
            {
                if(id_first == 0)
                {
                    sets[pos] = DFA_map[num];
                    id_first = 1;
                    for(const auto& node:DFA_map[num])
                        map[node] = pos;
                }
                else
                {
                    sets.emplace_back(DFA_map[num]);
                    int now_size = sets.size() - 1;
                    for(const auto& node:DFA_map[num])
                        map[node] = now_size;
                }
            }
            return true;
        }
        id_set.clear();
    }
    return false;
}

void DFA::get_all_DFA_nodes(DFA_node* node,unordered_set<DFA_node*>& set,vector<DFA_node*>& total_nodes)
{
    if(!node || set.find(node) != set.end())
        return;

    set.emplace(node);
    total_nodes.emplace_back(node);

    for(const auto& ptr:node->next_work_edges)
    {
        if(set.find(ptr) == set.end())
            get_all_DFA_nodes(ptr,set,total_nodes);
    }
    return;
}

void DFA::hopcraft()
{
    if(!root)   return;
    bool can_be_split = true;
    vector<vector<DFA_node*>> sets(2,vector<DFA_node*>());
    vector<DFA_node*> total_nodes;

    unordered_set<DFA_node*> set;
    unordered_map<DFA_node*,int> map;

    get_all_DFA_nodes(root,set,total_nodes);

    for(const auto& node:total_nodes)
    {
        int num = (node->state == ACCEPTABLE);
        sets[num].emplace_back(node);
        map[node] = num;
    }

    while(can_be_split)
    {
        can_be_split = false;
        int size = sets.size();
        for(int i=0;i<size;++i)
        {
            if(sets[i].empty()) continue;
            can_be_split = split(sets,sets[i],map,i);
            if(can_be_split)    break;
        }
    }

    int root_id = map[root];

    /*for(const auto& set:sets)
    {
        cout << "id: " ;
        for(const auto& node:set)
        {
            cout << node->id << ' ';
        }
        cout << endl;
    }*/

    unordered_set<char> chr_set;
    unordered_set<int>  work_id_set;
    unordered_map<int,DFA_node*> node_map;
    deque<DFA_node*> work_list;
    deque<int> work_id;
    int id_max = 0;


    if(map.find(root) != map.end())
    {
        auto node = new DFA_node(id_max++);
        root = node;
        work_list.emplace_back(node);
        work_id.emplace_back(root_id);
    }

    while(!work_list.empty())
    {
        chr_set.clear();

        auto now_node = work_list.front();
        work_list.pop_front();

        int id = work_id.front();
        work_id.pop_front();

        for(auto& node:sets[id])
        {
            if(node->state == ACCEPTABLE)    now_node->state = ACCEPTABLE;
            int size = node->next_values.size();
            for(int i=0;i<size;++i)
            {
                char chr = node->next_values[i];
                if(chr_set.find(chr) == chr_set.end())
                {
                    chr_set.emplace(chr);
                    int node_id = map[node->next_work_edges[i]];
                    DFA_node* ptr = nullptr;

                    if(node_map.find(node_id) != node_map.end())    ptr = node_map[node_id];
                    else
                    {
                        ptr = new DFA_node(id_max++);
                        node_map[node_id] = ptr;
                    }

                    now_node->next_values.emplace_back(chr);
                    now_node->next_work_edges.emplace_back(ptr);

                    if(work_id_set.find(node_id) == work_id_set.end())
                    {
                        work_list.emplace_back(ptr);
                        work_id.emplace_back(node_id);
                        work_id_set.emplace(node_id);
                    }
                }
            }
        }
    }

    for(auto& node:total_nodes)
        free(node);
    node_cnt = id_max;
}

void DFA::delete_all_nodes()
{
    if(!root)   return;
    unordered_set<DFA_node*> set;
    vector<DFA_node*> total_nodes;

    get_all_DFA_nodes(root,set,total_nodes);
    for(const auto& node:set)
        delete(node);
}

#endif // DFA_H
