#include <string.h>

#include "operation.h"
#include "myhash.h"
#include "printer.h"
#include "reader.h"

int lisp_strcmp(Lisp lisp, sexpr_t a, const char *str)
{
    if (atomp(a) && (atomtype(a) == T_SYMBOL) && !strcmp(atom2str(a), str))
        return 0;
    else
        return -1;
}

sexpr_t make_symbol(Lisp lisp, const char *sym, hashbucket *pbucket)
{
    sexpr_t ret = lisp_cons(lisp, NULL, NULL);
    setatom_sym(ret, intern(sym, pbucket));
    return ret;
}

sexpr_t lisp_list(Lisp lisp, sexpr_t a, sexpr_t b)
{
    sexpr_t ret = GC_malloc(sizeof(word));
    ret = lisp_cons(lisp, a, lisp_cons(lisp, b, NULL));
    return ret;
}

sexpr_t lisp_cons(Lisp lisp, sexpr_t a, sexpr_t b)
{
    sexpr_t ret = GC_malloc(sizeof(word));
    make_expr(ret, a, b);
    return ret;
}

sexpr_t lisp_eq(Lisp lisp, sexpr_t a, sexpr_t b)
{
    if (a->a.word == NULL && a->d.word == NULL && b->a.word == NULL && b->a.word == NULL) // 二者都是空表
        return &lisp->t;
    if (atomp(a) && atomp(b) && (a->a.umask == b->a.umask)) // 都是atom且umask相同
    {
        int atom_type = atomtype(a);
        // 若是数字
        if (atom_type == T_NUM)
        {
            if (a->d.num == b->d.num)
                return &lisp->t;
            else
                return &lisp->nill;
        }
        else
        {
            // 若是string或symbol
            if ((atom_type == T_STR) || (atom_type == T_SYMBOL))
            {
                if (a->d.str == b->d.str)
                    return &lisp->t;
                else
                    return &lisp->nill;
            }
            else
            {
                // 若是function
                if (atom_type == T_FUNCTION)
                {
                    if (a->d.func == b->d.func)
                        return &lisp->t;
                    else
                        return &lisp->nill;
                }
                else
                {
                    // 若是expression
                    if (atom_type == T_SEXPR)
                    {
                        if (a->d.word == b->d.word)
                            return &lisp->t;
                        else
                            return &lisp->nill;
                    }
                    else
                    { // 最后检测是否是lisp->t
                        if ((atom_type == 0) && (a->d.num == b->d.num))
                            return &lisp->t;
                        else
                            return &lisp->nill;
                    }
                }
            }
        }
    }
    else // 否则就不等
        return &lisp->nill;
}

sexpr_t lisp_assoc(Lisp lisp, sexpr_t e, sexpr_t a)
{
    if (a == &lisp->nill)
    {
        printf("unknown symbol:");
        print_atom(lisp, e);
        puts("");
        return a;
    }
    if (lisp_eq(lisp, CAAR(a), e) == &lisp->t)
    {
        return CADAR(a);
    }
    else
    {
        return lisp_assoc(lisp, e, CDR(a));
    }
}

sexpr_t lisp_eval(Lisp lisp, sexpr_t e, sexpr_t a)
{
    if (e == NULL)
    {
        return NULL;
    }
    if (nullp(e)) // 处理空list
        return &lisp->nill;
    if (atomp(e)) // 若输入是atom
    {
        if (atomtype(e) == T_SYMBOL) // 如果是symbol，对照环境返回相应的值
            return lisp_assoc(lisp, e, a);
        else // 否则直接返回该word
            return e;
    }
    else if (atomp(CAR(e))) // 处理几个基本操作
    {
        if (atomtype(CAR(e)) == T_SYMBOL)
        {
            // apply
            const char *ast = atom2str(CAR(e));
            if (!strcmp(ast, lisp->squote)) // quote
            {
                return CADR(e);
            }
            else if (!strcmp(ast, lisp->satom)) // atom
            {
                sexpr_t ret = lisp_eval(lisp, CADR(e), a);
                if (atomp(ret) || nullp(ret)) // 原子或空表
                    return &lisp->t;
                else
                    return &lisp->nill;
            }
            else if (!strcmp(ast, lisp->seq)) // eq
            {
                sexpr_t temp1 = lisp_eval(lisp, CADR(e), a);
                sexpr_t temp2 = lisp_eval(lisp, CADDR(e), a);
                // return lisp_eq(lisp, lisp_eval(lisp, CADR(e), a),
                //              lisp_eval(lisp, CADDR(e), a));
                return lisp_eq(lisp, temp1, temp2);
            }
            else if (!strcmp(ast, lisp->scar)) // car
            {
                return CAR(lisp_eval(lisp, CADR(e), a));
            }
            else if (!strcmp(ast, lisp->scdr)) // cdr
            {
                return CDR(lisp_eval(lisp, CADR(e), a));
            }
            else if (!strcmp(ast, lisp->scons)) // cons
            {
                return lisp_cons(lisp, lisp_eval(lisp, CADR(e), a),
                                 lisp_eval(lisp, CADDR(e), a));
            }
            else if (!strcmp(ast, lisp->scond)) // cond
            {
                sexpr_t branches = CDR(e);
                while (!nullp(branches))
                {
                    sexpr_t judge = lisp_eval(lisp, CAAR(branches), a);
                    if (!nullp(judge)) // 如果满足条件就返回对应的值
                        return lisp_eval(lisp, CADAR(branches), a);
                    else // 否则继续考察下一个条件
                        branches = CDR(branches);
                }
                // 如果全部条件都判断完都不符合，则表明cond表达式错误
                printf("cond error");
                return &lisp->nill;
            }
            else // 没匹配上基本操作则从环境中匹配，重新eval
            {
                return lisp_eval(lisp,
                                 lisp_cons(lisp,
                                           lisp_assoc(lisp, CAR(e), a),
                                           CDR(e)),
                                 a);
            }
        }
    }
    else if (!lisp_strcmp(lisp, CAAR(e), lisp->slabel)) // 处理label函数调用
    {
        return lisp_eval(lisp,
                         lisp_cons(lisp, CADDAR(e), CDR(e)),
                         lisp_cons(lisp, lisp_list(lisp, CADAR(e), CAR(e)), a));
    }
    else if (!lisp_strcmp(lisp, CAAR(e), lisp->sfn)) // 处理lambda函数调用
    {
        return lisp_eval(lisp,
                         CADDAR(e),
                         lisp_append(lisp,
                                     lisp_pair(lisp,
                                               CADAR(e),
                                               lisp_evlis(lisp, CDR(e), a)),
                                     a));
    }
    else
    { // 出错处理
        printf("eval error:");
        return &lisp->nill;
    }
}
sexpr_t lisp_evlis(Lisp lisp, sexpr_t m, sexpr_t env)
{
    if (nullp(m))
        return &lisp->nill;
    else
        return lisp_cons(lisp,
                         lisp_eval(lisp, CAR(m), env),
                         lisp_evlis(lisp, CDR(m), env));
}
sexpr_t lisp_pair(Lisp lisp, sexpr_t x, sexpr_t y)
{
    if (nullp(x) && nullp(y))
        return lisp_cons(lisp, NULL, NULL);
    else if (!atomp(x) && !atomp(y))
    {
        return lisp_cons(lisp,
                         lisp_list(lisp, CAR(x), CAR(y)),
                         lisp_pair(lisp, CDR(x), CDR(y)));
    }
    else
    {
        printf("lisp_pair failed");
        return &lisp->nill;
    }
}
sexpr_t lisp_append(Lisp lisp, sexpr_t x, sexpr_t y)
{
    if (nullp(x))
        return y;
    else
    {
        return lisp_cons(lisp,
                         CAR(x),
                         lisp_append(lisp, CDR(x), y));
    }
}

void lisp_loop(Lisp lisp, sexpr_t word, hashbucket *pbucket)
{
    if (word == NULL)
    {
        return;
    }
    while (1)
    {
        if (nullp(word))
        {
            print_atom(lisp, word);
        }
        else if (atomp(word)) // 打印atom
        {
            print_atom(lisp, word);
        }
        else // 打印expression
        {
            print_expr(lisp, word);
        }
        printf("\n"); // 处理完第一句，继续read eval 循环
        word = lisp_read(lisp, pbucket);
        if (word == NULL)
            return;
        word = lisp_eval(lisp, word, lisp->env);
    }
}