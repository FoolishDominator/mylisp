#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "fileio.h"
#include "myhash.h"
#include "interpreter.h"
#include "reader.h"
#include "operation.h"

hashbucket buckets[NBUCKET];
// static u8 *_gc_heap = 0;

int main(int argc, char *argv[])
{
        // init interned
        init_hashbucket(buckets, NBUCKET);
        GC_INIT();

        // 执行方法：./LISP FILENAME 解析lisp文件
        if (argc != 2)
        {
                printf("Usage: %s FILE\n", argv[0]);
                return -1;
        }

        // 为interpreter结构体分配内存
        Lisp lisp = (Lisp)malloc(sizeof(interpreter));
        // 初始化interpreter，打开文件流
        int ret = init_interpreter(lisp, buckets, argv[1]);
        if (ret != 1)
        {
                printf("init interpreter failed");
                return -1;
        }

        sexpr_t list = lisp_read(lisp, buckets);

        list = lisp_eval(lisp, list, lisp->env);

        lisp_loop(lisp, list, buckets);

        printf("\n");
        lisp->fctx.close_stream(&lisp->fctx);

        return 0;
}