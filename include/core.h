#ifndef _CORE_H_
#define _CORE_H_
// 定义lisp的基本operations

#include "interpreter.h"

// 定义一些基本宏

#define CAR(w) ((w)->a.word)
#define CDR(w) ((w)->d.word)
#define CAAR(w) ((w)->a.word->a.word)
#define CADR(w) ((w)->d.word->a.word)
#define CDDR(w) ((w)->d.word->d.word)
#define CADDR(w) ((w)->d.word->d.word->a.word)
#define CADAR(w) ((w)->a.word->d.word->a.word)
#define CADDAR(w) ((w)->a.word->d.word->d.word->a.word)

// 判断atom,正确返回1，反之返回0
int atomp(word *w);
// 判断null，正确返回1，反之返回0
int nullp(word *w);
// 解析判断atom类型
//  返回输入atom的类型
int atomtype(word *atom);
// 将atom解析为string；
const char *atom2str(word *atom);
// 将atom解析为num
int atom2num(word *atom);
// 将atom解析为expression
word *atom2expr(word *atom);
// 将atom解析为function
parserfn_t atom2func(word *atom);

// 设置atom类型
// 设置atom为string
void setatom_str(word *atom, const char *str);
// 设置atom为num
void setatom_num(word *atom, int n);
// 设置atom为expression
void setatom_expr(word *atom, word *expr);
// 设置atom为function
void setatom_func(word *atom, parserfn_t fn);
// 设置atom为symbol
void setatom_sym(word *atom, const char *str);

// 构建expression
void make_expr(word *expr, word *ca, word *cd);
// 设置ca的值
void set_ca(word *expr, word *ca);
// 设置cd的值
void set_cd(word *expr, word *cd);

#endif