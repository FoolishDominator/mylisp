#include <string.h>

#include "interpreter.h"
#include "myhash.h"
#include "operation.h"

// 初始化成功返回1，失败返回-1
int init_interpreter(Lisp lisp, hashbucket *pbucket, const char *filename)
{
    memset(lisp, 0, sizeof(interpreter));

    // 绑定lisp关键字
    lisp->squote = intern("quote", pbucket);
    lisp->scar = intern("car", pbucket);
    lisp->scdr = intern("cdr", pbucket);
    lisp->satom = intern("atom", pbucket);
    lisp->scond = intern("cond", pbucket);
    lisp->scons = intern("cons", pbucket);
    lisp->seq = intern("eq", pbucket);
    lisp->slabel = intern("label", pbucket);
    lisp->sfn = intern("lambda", pbucket);
    lisp->ssetq = intern("setq", pbucket);

    // 打开文件流，绑定lisp的fctx结构
    if (!init_file_stream(&lisp->fctx, filename))
    {
        return -1;
    }
    // 初始化环境为“()”
    lisp->t.a.umask = 1;
    lisp->t.d.num = 1;
    lisp->nill.a.word = NULL;
    lisp->nill.d.word = NULL;

    lisp->env = lisp_cons(lisp,
                          lisp_list(lisp,
                                    make_symbol(lisp, "nil", pbucket),
                                    &lisp->nill),
                          lisp_cons(lisp,
                                    lisp_list(lisp,
                                              make_symbol(lisp, "true", pbucket),
                                              &lisp->t),
                                    &lisp->nill));
    lisp->call = &lisp->nill;
    lisp->macros = &lisp->nill;

    return 1;
}