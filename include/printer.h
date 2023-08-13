#ifndef _PRINTER_H_
#define _PRINTER_H_

#include "interpreter.h"

void print_atom(Lisp lisp, sexpr_t atom);

void print_expr(Lisp lisp, sexpr_t word);

#endif