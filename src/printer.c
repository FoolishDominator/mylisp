#include "printer.h"
#include "core.h"
#include "operation.h"

void print_atom(Lisp lisp, sexpr_t atom)
{
    if (nullp(atom))
        printf("false");
    else if (lisp_eq(lisp, atom, &lisp->t) == &lisp->t)
        printf("true");
    else
    {
        switch (atomtype(atom))
        {
        case T_NUM:
            printf("%d", atom2num(atom));
            break;
        case T_STR:
        case T_SYMBOL:
            printf("%s", atom2str(atom));
            break;
        case T_SEXPR:
            printf("%p", atom2expr(atom));
            break;
        case T_FUNCTION:
            printf("function#%p", atom2func(atom));
            break;
        default:
            printf("<strange atom>");
            break;
        }
    }
}

void print_expr(Lisp lisp, sexpr_t word)
{
    printf("(");

    while (1)
    {
        if (nullp(CAR(word)))
            printf("nil");
        else if (atomp(CAR(word)))
            print_atom(lisp, CAR(word));
        else
            print_expr(lisp, CAR(word));

        if (!nullp(CDR(word))) // 如果有dr则添个空格
            printf(" ");
        word = CDR(word);
        if (nullp(word)) // 读完了就停止
            break;
    }
    printf(")");
}