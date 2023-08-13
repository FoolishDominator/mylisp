#ifndef _MYHASH_H_
#define _MYHASH_H_

#define NBUCKET 1023

#include <stdlib.h>

typedef unsigned char u8;
static size_t _gc_size = 0;
static size_t _gc_free = 0;
static u8 *_gc_heap = 0;

typedef struct hashbucket_t
{
    const char *str;
    struct hashbucket_t *next;
} hashbucket;

// 返回一个字符的哈希值
unsigned int hash_str(const char *str);
void init_hashbucket(hashbucket *hbPtr, int nbucket);
void GC_INIT();
void *GC_malloc(size_t len);
// 分配空间并字符串复制，返回字符串首地
const char *gc_strdup(const char *str);
// 向哈希桶中存入字符串并返回字符串地址
const char *intern(const char *str, hashbucket *pbucket);
#endif