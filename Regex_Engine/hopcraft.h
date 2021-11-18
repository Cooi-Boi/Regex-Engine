#ifndef HOPCRAFT_H
#define HOPCRAFT_H

#include "DFA.h"
#include "DFA_node.h"
#include "unordered_set"


void Split(DFA_node* node);
void Get_All_DFA_Nodes(DFA_node* DFA,deque<DFA*>& total_DFA_nodes);
void Hopcraft(DFA* DFA);

#endif // HOPCRAFT_H
