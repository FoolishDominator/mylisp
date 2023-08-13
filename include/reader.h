#ifndef _READER_H_
#define _READER_H_

#include <ctype.h>
#include <string.h>
#include "interpreter.h"

// 顶层函数
sexpr_t lisp_read(Lisp I, hashbucket *pbucket);

// 中间函数
int get_char(Lisp lisp);
void put_char(Lisp lisp, int c);
sexpr_t lisp_readlist(Lisp lisp, hashbucket *pbucket);
sexpr_t lisp_readatom(Lisp lisp, hashbucket *pbucket);

int issymbol(int ch);

#endif