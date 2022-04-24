#include <iostream>
#include <iomanip>

#include "asttree.cpp"
#include "asttree_node.cpp"
#include "dfa.cpp"
#include "dfa_node.cpp"
#include "nfa.cpp"
#include "nfa_node.cpp"
#include "retree.cpp"

int main(void)
{
    cout << "------------------------------------------------------------------------\n" <<
            "Love 6's Regular Expression Engines (Support closure\\concat\\select\\Express)^^ " << endl <<
            "Love 6's Blog: https://love6.blog.csdn.net/" << endl                        <<
            "the tiny Regex Engines is updating(maybe~ haha)" << endl                    <<
            "-------------------------------------------------------------------------\n"<< endl <<
            "input Regular Expression(only support correct expression): " << endl;

    string re;
    cin >> re;
    ReTree ReTree(re);

    cout << "\ninput Test String Longest_Prefix_Match (Ctrl + Z Stop): \n";
    while(cin >> re)
        ReTree.re_match(re);

    return 0;
}

