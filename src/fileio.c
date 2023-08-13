#include "fileio.h"

// 关闭文件流
void file_close_stream(fctx *s)
{
        fclose(s->fstream);
}

// 从文件流中读取字符
int file_get_char(fctx *s)
{
        return fgetc(s->fstream);
}

// 将字符推回文件流
void file_put_char(fctx *s, int ch)
{
        ungetc(ch, s->fstream);
}

// 初始化fctx结构体（打开文件）
int init_file_stream(fctx *pFliecontext, const char *filename)
{
        pFliecontext->fstream = fopen(filename, "r");
        if (!pFliecontext->fstream)
                return 0;
        pFliecontext->close_stream = &file_close_stream;
        pFliecontext->get_char = &file_get_char;
        pFliecontext->put_char = &file_put_char;
        return 1;
}
