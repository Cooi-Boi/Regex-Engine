#include <iostream>
#include <iomanip>
#include "ReTree.h"

int main(void)
{
    ReTree ReTree;
    string re;
    cout << "------------------------------------------------------------------------\n";
    cout << "Love 6's Regular Expression Engines (Support closure\\concat\\select\\Express)^^ \n";
    cout << "Love 6's Blog: https://love6.blog.csdn.net/\n";
    cout << "the tiny Regex Engines is updating(maybe~ haha)\n";
    cout << "-------------------------------------------------------------------------\n\n";

    cout << "input Regular Expression(only support correct expression): \n";

    cin >> re;
    ReTree.build(re);

    cout << "\ninput Test String Longest_Prefix_Match (Ctrl + Z Stop): \n";
    while(cin >> re)
        ReTree.re_match(re);

    return 0;
}
