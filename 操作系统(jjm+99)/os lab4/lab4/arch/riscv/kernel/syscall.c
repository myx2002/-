#include "syscall.h"
#include "printk.h"
#include "proc.h" 

//sys_getpid()函数的实现
uint64 sys_getpid()
{
    return getcur_id();
}

//sys_write()函数的实现
uint64 sys_write(unsigned int fd, const char* buf, size_t count)
{
    uint64 re_num;
    char* add_start = (char*)buf;
    //借助printk实现字符串的输出
    re_num = printk("%s",add_start);
    return re_num;
}