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
    
    /*unsigned long num = csr_read(sscratch);
    printk("%x\n",num);
    csr_write(sscratch,1);
    num = csr_read(sscratch);
    printk("%x\n",num);*/

    //printk("%lx\n",_stext);
    uint64 now_va = _stext;
    uint64 vpn2 = (now_va >> 30) & 0x1ff;
    uint64 vpn1 = (now_va >> 21) & 0x1ff;
    uint64 vpn0 = (now_va >> 12) & 0x1ff;
    uint64 pte_1 = swapper_pg_dir[vpn2];
    uint64* pgtbl_1;
    pgtbl_1 = (uint64*) (((pte_1 & 0x003FFFFFFFFFFC00) << 2) + PA2VA_OFFSET);
    uint64 pte_2 = pgtbl_1[vpn1];
    uint64* pgtbl_2;
    pgtbl_2 = (uint64*) (((pte_2 & 0x003FFFFFFFFFFC00) << 2) + PA2VA_OFFSET);
    uint64 pte_3 = pgtbl_2[vpn0];
    //printk("%lx\n",pte_3 & 0xf);

    //printk("%lx\n",_srodata);
    uint64 now_va_d = _srodata;
    uint64 vpn2_d = (now_va_d >> 30) & 0x1ff;
    uint64 vpn1_d = (now_va_d >> 21) & 0x1ff;
    uint64 vpn0_d = (now_va_d >> 12) & 0x1ff;
    uint64 pte_1_d = swapper_pg_dir[vpn2_d];
    uint64* pgtbl_1_d;
    pgtbl_1_d = (uint64*) (((pte_1_d & 0x003FFFFFFFFFFC00) << 2) + PA2VA_OFFSET);
    uint64 pte_2_d = pgtbl_1_d[vpn1_d];
    uint64* pgtbl_2_d;
    pgtbl_2_d = (uint64*) (((pte_2_d & 0x003FFFFFFFFFFC00) << 2) + PA2VA_OFFSET);
    uint64 pte_3_d = pgtbl_2_d[vpn0_d];
    //printk("%lx\n",pte_3_d & 0xf);


    schedule();
    test(); // DO NOT DELETE !!!
    

	return 0;
}
