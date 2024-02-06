#include "printk.h"
#include "sbi.h"
#include "defs.h"
#include "types.h"

extern void test();
//通过查看其地址对映flag来看属性是否设置正确
extern char _stext[];
extern char _srodata[];
extern unsigned long swapper_pg_dir[];

int start_kernel() {
    printk("[S-MODE] 2022");
    printk(" Hello RISC-V\n");


    schedule();
    test(); // DO NOT DELETE !!!
    

	return 0;
}
