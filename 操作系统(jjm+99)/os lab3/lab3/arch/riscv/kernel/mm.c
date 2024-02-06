#include "defs.h"
#include "string.h"
#include "mm.h"

#include "printk.h"
//简单的物理内存管理接口
extern char _ekernel[];
extern char _skernel[];

struct {
    struct run *freelist;
} kmem;

//申请4KB的物理页
uint64 kalloc() {
    struct run *r;

    r = kmem.freelist;
    kmem.freelist = r->next;
    
    memset((void *)r, 0x0, PGSIZE);
    return (uint64) r;
}

void kfree(uint64 addr) {
    struct run *r;

    // PGSIZE align 
    addr = addr & ~(PGSIZE - 1);

    memset((void *)addr, 0x0, (uint64)PGSIZE);

    r = (struct run *)addr;
    //printk("%lx\n",r);
    r->next = kmem.freelist;
    kmem.freelist = r;

    return ;
}

void kfreerange(char *start, char *end) {
    char *addr = (char *)PGROUNDUP((uint64)start);
    //以页面大小 (PGSIZE) 的间隔循环，每次调用 kfree() 来释放一个页面
    for (; (uint64)(addr) + PGSIZE <= (uint64)end; addr += PGSIZE) {
        kfree((uint64)addr);
    }
}

void mm_init(void) {
    //调用 kfreerange() 来释放从 _ekernel 到 PHY_END 之间的内核内存页
    //_ekernel已被改为虚拟内存地址
    kfreerange(_ekernel, (char *)(PHY_END + PA2VA_OFFSET));
    //printk("%d",PHY_END + PA2VA_OFFSET - (uint64)_ekernel);
    //kfreerange(_skernel, (char *)(_skernel + PA2VA_OFFSET));
    printk("...mm_init done!\n");
}
