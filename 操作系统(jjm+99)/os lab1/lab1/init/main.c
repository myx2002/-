#include "printk.h"
#include "sbi.h"
#include "defs.h"
#include "types.h"

extern void test();

int start_kernel() {
    printk("2022");
    printk(" Hello RISC-V\n");
    unsigned long num = csr_read(sscratch);
    printk("%x\n",num);
    csr_write(sscratch,1);
    num = csr_read(sscratch);
    printk("%x\n",num);

    test(); // DO NOT DELETE !!!
    

	return 0;
}
