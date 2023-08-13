#include <stdio.h>
#include "myhash.h"
#include <string.h>

unsigned int hash_str(const char *str)
{
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash;
}

void init_hashbucket(hashbucket *hbPtr, int nbucket)
{
    for (int i = 0; i < nbucket; i++)
    {
        hbPtr[i].str = NULL;
        hbPtr[i].next = NULL;
    }
}

void GC_INIT()
{
    // 分配64k空间
    _gc_size = _gc_free = 64 * 1024;
    _gc_heap = (u8 *)malloc(_gc_size);
}

void *GC_malloc(size_t len)
{
    len = (len + (0x10 - 1)) & -0x10;
    if (len > _gc_free)
    {
        fprintf(stderr, "heap exhaustion, compile with GC support\n");
        abort();
    }
    _gc_free -= len;
    return _gc_heap + _gc_free;
}

const char *gc_strdup(const char *str)
{
    int len = strlen(str);
    void *buf = GC_malloc(len + 1);
    memcpy(buf, str, len + 1);
    return buf;
}

const char *intern(const char *str, hashbucket *pbucket)
{
    unsigned int hashval = hash_str(str);
    hashbucket *hb = NULL;
    hashbucket *hb2 = NULL;
    // hb指向该str应在的bucket
    hb = pbucket + (hashval % NBUCKET);
    if (hb->str == NULL) // 桶中无string
    {
        hb->str = gc_strdup(str);
        return hb->str;
    }
    else if (strcmp(hb->str, str) == 0)
    { // 桶中恰好为待存字符串
        return hb->str;
    }
    // 其余情况：遍历检查该bucket的next，若已经存储过该字符串，直接返回，若没有则重新分配空间
    while (hb->next != NULL)
    {
        if (strcmp(hb->str, str))
        {
            return hb->str;
        }
        hb = hb->next;
    }
    hb2 = GC_malloc(sizeof(hashbucket));
    hb->next = hb2;
    hb2->next = NULL;
    hb2->str = gc_strdup(str);
    return hb2->str;
}