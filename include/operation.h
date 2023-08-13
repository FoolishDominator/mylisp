#ifndef _OPERATION_H_
#define _OPERATION_H_

#include "core.h"

// note:(list a b c)=(cons a (cons b (cons c ())))
// 代码仅实现了两参数的list

//  (list a (b c d))=(a (b c d))
sexpr_t lisp_list(Lisp lisp, sexpr_t a, sexpr_t b);
// 实际仅仅是make_expr的包裹函数,其中进行了内存分配
// (cons a (b c d))=(a b c d)
sexpr_t lisp_cons(Lisp lisp, sexpr_t a, sexpr_t b);
sexpr_t lisp_eq(Lisp lisp, sexpr_t a, sexpr_t b);
// 接受一个原子e和一个由pair.创建的形式的列表a，并返回a中第一个列表的第二个元素，其第一个元素是x。
sexpr_t lisp_assoc(Lisp lisp, sexpr_t e, sexpr_t a);
// eval算法来自root of lisp;e是要求值的表达式，a是环境（即一些赋给原子的值构成的表）
sexpr_t lisp_eval(Lisp lisp, sexpr_t e, sexpr_t a);
// 对照环境将自变量求值，添加到环境中。算法来自root of lisp
sexpr_t lisp_evlis(Lisp lisp, sexpr_t m, sexpr_t env);
// 接受两个长度相同的列表（不能是atom），并返回一个双元素列表的列表，其中包含每个列表中连续的元素对。算法来自root of lisp
sexpr_t lisp_pair(Lisp lisp, sexpr_t x, sexpr_t y);
// 接受两个列表并返回它们的串联。算法来自root of lisp
sexpr_t lisp_append(Lisp lisp, sexpr_t x, sexpr_t y);
// 构建一个symbol的word，内部会进行内存分配，返回一个word*
sexpr_t make_symbol(Lisp lisp, const char *sym, hashbucket *pbucket);
// 比较atom是否为symbol且是否内容与目标string一致;一致返回0，不一致返回-1。
int lisp_strcmp(Lisp lisp, sexpr_t a, const char *str);
void lisp_loop(Lisp lisp, sexpr_t word, hashbucket *pbucket);
#endif