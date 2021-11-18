#ifndef GLOBAL_H
#define GLOBAL_H

using namespace std;

enum states{UNACCEPTABLE,ACCEPTABLE};

//定义 AST中 每个节点的状态
//             epsilon     字符char    选择|     连接+    闭包*
enum exp_kind {EXP_EPSILON,EXP_CHR,EXP_SELECT,EXP_CONCAT,EXP_CLOSURE};

#endif // GLOBAL_H
