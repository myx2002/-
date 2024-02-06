#ifndef _DEFS_H
#define _DEFS_H

#include "types.h"

#define PHY_START 0x0000000080000000 //程序起始地址，需要清楚数据的起始地址
#define PHY_SIZE  128 * 1024 * 1024 // 128MB，QEMU 默认内存大小
#define PHY_END   (PHY_START + PHY_SIZE) 

#define PGSIZE 0x1000 // 4KB
#define PGROUNDUP(addr) ((addr + PGSIZE - 1) & (~(PGSIZE - 1)))
#define PGROUNDDOWN(addr) (addr & (~(PGSIZE - 1)))

//lab3修改内容
//spensbi偏移量
#define OPENSBI_SIZE (0x200000)

//虚拟地址开始位置
#define VM_START (0xffffffe000000000)
//虚拟地址结束位置
#define VM_END   (0xffffffff00000000)
//虚拟地址累计占存
#define VM_SIZE  (VM_END - VM_START)
//虚拟地址与实际地址间的偏移量
#define PA2VA_OFFSET (VM_START - PHY_START)
//end lab3

#define csr_read(csr)                       \
({                                          \
    register uint64 __v;                    \
    /* unimplemented */                     \
    /*使用伪代码 csrr 获得寄存器中的数据*/    \
    asm volatile ("csrr %0," #csr           \
                    :"=r" (__v));             \
    __v;                                    \
})

#define csr_write(csr, val)                         \
({                                                  \
    uint64 __v = (uint64)(val);                     \
    asm volatile ("csrw " #csr ", %0"               \
                    : : "r" (__v)                   \
                    : "memory");                    \
})

#endif
