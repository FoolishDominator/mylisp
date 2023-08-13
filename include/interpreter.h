// 定义interpreter相关数据结构及函数
#ifndef _INTERPRETER_H_
#define _INTERPRETER_H_
#include "fileio.h"
#include "myhash.h"

// 表明word的类型
#define T_SEXPR 0
#define T_NUM 0x10
#define T_STR 0x20
#define T_SYMBOL 0x30
#define T_FUNCTION 0x40

typedef struct word_t *
    sexpr_t;                        // expression的列表
typedef struct interpreter_t *Lisp; // lisp解释器
typedef sexpr_t (*parserfn_t)(Lisp, sexpr_t, sexpr_t);

typedef union
{
    unsigned int umask;  // 若此word是atom时选用，表明类型
                         // 最低一位为1表示atom，高位区分具体类型
    struct word_t *word; // 若此word仍为list时选用，指向该list的car操作后的结果
} address_t;

typedef union
{
    struct word_t *word; // 若此word仍为list时选用，指向该list的cdr操作后的结果
    // 以下均为word是atom时选用，表明atom具体的值
    int num;
    const char *str;
    parserfn_t func;
} decrement_t;

// lisp中的word
typedef struct word_t
{
    address_t a;
    decrement_t d;
} word;

typedef struct interpreter_t
{
    fctx fctx;
    struct word_t t;
    struct word_t nill;
    sexpr_t env;    /* assoclist of variables */
    sexpr_t call;   /* when in a function, refers to the call form itself */
    sexpr_t macros; /* assoclist of macros */
    const char *squote;
    const char *scar;
    const char *scdr;
    const char *satom;
    const char *scond;
    const char *scons;
    const char *seq;
    const char *slabel;
    const char *sfn;
    const char *ssetq;
} interpreter;

int init_interpreter(Lisp lisp, hashbucket *pbucket, const char *filename);

#endif