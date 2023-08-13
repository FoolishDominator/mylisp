// 文件io相关函数。
#ifndef _FILEIO_H_
#define _FILEIO_H_

#include <stdio.h>

typedef struct filecontext
{
        FILE *fstream;
        void (*close_stream)(struct filecontext *);
        int (*get_char)(struct filecontext *);
        void (*put_char)(struct filecontext *, int);
} fctx;

int init_file_stream(fctx *pFilecontext, const char *filename);

#endif //_FILEIO_H_
