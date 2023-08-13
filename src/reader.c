#include "reader.h"
#include "operation.h"
#include "stdlib.h"

sexpr_t lisp_read(Lisp lisp, hashbucket *pbucket)
{
    int ch;
    sexpr_t ret = NULL;
    ch = get_char(lisp);
    if (ch == EOF)
        return NULL;
    while (ch)
    {
        if (isblank(ch) || ch == '\n')
        {
        }
        else if (ch == '(') // 读入一个list
        {
            put_char(lisp, ch);
            ret = lisp_readlist(lisp, pbucket);
            break;
        }
        else if (ch == '\'') // "'"符号打头看作quote操作符的简写，需要单独处理
        {
            static const char *squote = NULL;
            sexpr_t quotelist;
            if (!squote)
            {
                squote = intern("quote", pbucket);
            }
            quotelist = lisp_cons(lisp, NULL, NULL);
            setatom_sym(quotelist, squote);
            ret = lisp_cons(lisp, quotelist, lisp_cons(lisp, lisp_read(lisp, pbucket), NULL));
            break;
        }
        else // 其余情况均视作读入atom（可能有些问题，待商榷）
        {
            put_char(lisp, ch);
            ret = lisp_readatom(lisp, pbucket);
            break;
        }
        ch = get_char(lisp); // 读取下一个list或atom...
    }
    return ret;
}

sexpr_t lisp_readlist(Lisp lisp, hashbucket *pbucket)
{
    int ch;
    ch = get_char(lisp);
    if (ch != '(')
        return NULL;
    // 构建该list的数据结构
    sexpr_t head, current, next;
    // head为该list的头部，为该函数的返回结果
    head = lisp_cons(lisp, NULL, NULL);
    // current为read进行到的word位置
    current = NULL;
    // next即为下一个待填入的数据结构
    next = head;

    ch = get_char(lisp);
    // 开始读取该list
    while (ch)
    {
        if (isspace(ch))
        {
            // 空格就跳过
        }
        else if (ch == ')')
        {
            // 右半括号就停止读取
            break;
        }
        else
        {
            put_char(lisp, ch);                        // 推回符号
            current = next;                            // 开始翻译list中下一个元素
            set_ca(current, lisp_read(lisp, pbucket)); // 从car cdr的角度理解
            next = lisp_cons(lisp, NULL, NULL);
            set_cd(current, next);
        }
        ch = get_char(lisp);
    }
    return head;
}

sexpr_t lisp_readatom(Lisp lisp, hashbucket *pbucket)
{
    int ch;
    char buffer[128];
    // 读取字符
    ch = get_char(lisp);
    if (ch == EOF)
        return NULL;
    if (isdigit(ch)) // 解析数字
    {
        int i = 0;
        memset(buffer, 0, sizeof(buffer));
        while (isdigit(ch))
        {
            buffer[i++] = ch;
            ch = get_char(lisp);
        }
        buffer[i] = 0;
        if ((ch != ' ') && (ch != ')') && (ch != EOF) && (ch != '\n'))
        { // 排除“342r”之类的情况
            printf("not a number!");
            return NULL;
        }
        put_char(lisp, ch); // 重新推回文件流
        sexpr_t atom = lisp_cons(lisp, NULL, NULL);
        setatom_num(atom, atoi(buffer));
        return atom;
    }
    else if (issymbol(ch)) // 解析symbol
    {
        int i = 0;
        memset(buffer, 0, sizeof(buffer));
        while (issymbol(ch) || isdigit(ch))
        {
            buffer[i++] = ch;
            ch = get_char(lisp);
        }
        buffer[i] = 0;
        put_char(lisp, ch); // 重新推回文件流
        sexpr_t atom = lisp_cons(lisp, NULL, NULL);
        setatom_sym(atom, intern(buffer, pbucket));
        return atom;
    }
    else if (ch == '"') // 解析字符串
    {
        int i = 0;
        memset(buffer, 0, sizeof(buffer));
        ch = get_char(lisp);
        while (ch != '"') // 一直读取到配对的引号
        {
            if (ch != '\\')
            {
                // 正常字符读取
                buffer[i++] = ch;
            }
            else
            {
                // 处理转义字符
                switch (get_char(lisp)) // 细分哪种转义字符
                {
                case 'n':
                    buffer[i++] = '\n';
                    break;
                case 'r':
                    buffer[i++] = '\r';
                    break;
                case '\\':
                    buffer[i++] = '\\';
                    break;
                case '"':
                    buffer[i++] = '"';
                    break;
                case 't':
                    buffer[i++] = '\t';
                    break;
                default:
                    buffer[i++] = ch;
                    break;
                }
            }
            ch = get_char(lisp);
        }
        buffer[i] = 0;
        sexpr_t atom = lisp_cons(lisp, NULL, NULL);
        char *str = GC_malloc(i);
        strcpy(str, buffer); // 分配内存保存string的值，因为buffer是临时的
        setatom_str(atom, str);
        return atom;
    }
    else // 错误处理
    {
        if (isgraph(ch))
        {
            printf("invalid character: %c\n", ch);
        }
        printf("invalid character | from lisp_readatom function.");
        return NULL;
    }
}

int issymbol(int ch)
{ // symbol即表示为字母或其他合法字符
    return isalpha(ch) ||
           (ch == '!') ||
           (ch == '%') ||
           (ch == '*') ||
           (ch == '+') ||
           (ch == '-') ||
           (ch == '_') ||
           (ch == '/') ||
           (ch == ':') ||
           (ch == '<') ||
           (ch == '>') ||
           (ch == '=') ||
           (ch == '?');
}

int get_char(Lisp lisp)
{
    return lisp->fctx.get_char(&lisp->fctx);
}

void put_char(Lisp lisp, int c)
{
    return lisp->fctx.put_char(&lisp->fctx, c);
}