#include "string.h"
//初始化一段内存空间
void *memset(void *dst, int c, uint64 n) {
    char *cdst = (char *)dst;
    for (uint64 i = 0; i < n; ++i)
        cdst[i] = c;

    return dst;
}
//memcpy函数
void *memcpy(void *to_cp, void *from_cp, uint64 num) 
{
    char* one = (char*)to_cp;
    char* two = (char*)from_cp;
    //根据字节数进行复制
    for (uint64 i = 0; i < num; i++)
    {
        one[i] = two[i];
    }
    return to_cp;
}