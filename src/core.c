#include "core.h"
#include "interpreter.h"

int atomp(word *w)
{
    return (w->a.umask & 1);
}
int nullp(word *w)
{
    return (w == NULL || (w->a.word == NULL && w->d.word == NULL));
}

int atomtype(word *atom)
{
    return (atom->a.umask & 0xfffffff0);
}

const char *atom2str(word *atom)
{
    if ((atomtype(atom) == T_STR) || (atomtype(atom) == T_SYMBOL))
    {
        return atom->d.str;
    }
    else
    {
        return NULL;
    }
}

int atom2num(word *atom)
{
    if (atomtype(atom) == T_NUM)
    {
        return atom->d.num;
    }
    else
    {
        return 0;
    }
}

word *atom2expr(word *atom)
{
    if (atomtype(atom) == T_SEXPR)
    {
        return atom->d.word;
    }
    else
    {
        return NULL;
    }
}

parserfn_t atom2func(word *atom)
{
    if (atomtype(atom) == T_FUNCTION)
    {
        return atom->d.func;
    }
    else
    {
        return NULL;
    }
}

void setatom_str(word *atom, const char *str)
{
    atom->a.umask = T_STR | 1;
    atom->d.str = str;
}

void setatom_num(word *atom, int n)
{
    atom->a.umask = T_NUM | 1;
    atom->d.num = n;
}

void setatom_expr(word *atom, word *expr)
{
    atom->a.umask = T_SEXPR | 1;
    atom->d.word = expr;
}

void setatom_func(word *atom, parserfn_t fn)
{
    atom->a.umask = T_FUNCTION | 1;
    atom->d.func = (fn);
}

void setatom_sym(word *atom, const char *str)
{
    atom->a.umask = T_SYMBOL | 1;
    atom->d.str = str;
}

void make_expr(word *expr, word *ca, word *cd)
{
    expr->a.word = ca;
    expr->d.word = cd;
}

void set_ca(word *expr, word *ca)
{
    expr->a.word = ca;
}

void set_cd(word *expr, word *cd)
{
    expr->d.word = cd;
}
