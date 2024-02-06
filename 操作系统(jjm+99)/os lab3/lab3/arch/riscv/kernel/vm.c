#include "defs.h"
#include "types.h"
#include "string.h"
#include "printk.h"

/* early_pgtbl: 用于 setup_vm 进行 1GB 的 映射。 */
unsigned long  early_pgtbl[512] __attribute__((__aligned__(0x1000)));

void setup_vm(void) {
    /* 
    1. 由于是进行 1GB 的映射 这里不需要使用多级页表 
    2. 将 va 的 64bit 作为如下划分： | high bit | 9 bit | 30 bit |
        high bit 可以忽略
        中间9 bit 作为 early_pgtbl 的 index
        低 30 bit 作为 页内偏移 这里注意到 30 = 9 + 9 + 12， 即我们只使用根页表， 根页表的每个 entry 都对应 1GB 的区域。 
    3. Page Table Entry 的权限 V | R | W | X 位设置为 1
    */

    
    uint64 va;
    /*
    //第一次偏移
    va = PHY_START;//0x0000000080000000
    uint64 index = (va & 0x0000007FC0000000) >> 30;
    //获得va对映的pte
    uint64 pte = PHY_START >> 12;
    //设置pte最低位参数
    early_pgtbl[index] = pte << 10 | 0xF;
    */

    //第二次偏移
    va = PHY_START + PA2VA_OFFSET;
    uint64 index_1 = (va & 0x0000007FC0000000) >> 30;
    //获得va对映的pte
    uint64 pte_1 = PHY_START >> 12;
    //设置pte最低位参数
    early_pgtbl[index_1] = pte_1 << 10 | 0xF;
    return;
}

/* swapper_pg_dir: kernel pagetable 根目录， 在 setup_vm_final 进行映射。 */
unsigned long  swapper_pg_dir[512] __attribute__((__aligned__(0x1000)));
//s前缀表示起始段（e表示结束）
//连续存放---->只取段头，段头差都暂且默认为同一区域
extern char _skernel[];
extern char _stext[];
extern char _srodata[];
extern char _sdata[];
extern char _ekernel[];

void setup_vm_final(void) {
    memset(swapper_pg_dir, 0x0, PGSIZE);

    // No OpenSBI mapping required
    //记录虚拟起始地址
    uint64 vms = (uint64)VM_START;
    //记录物理起始地址
    uint64 phs = (uint64)PHY_START;
    //记录OpenSBI偏移量
    uint64 sbis = (uint64)OPENSBI_SIZE;
    //记录.text的占用空间
    //_stext与_skernel标签位于同一个位置
    uint64 texts = (uint64)_srodata - (uint64)_stext;
    //记录.rodata段的占用空间
    uint64 rodatas = (uint64)_sdata - (uint64)_srodata;
    //最后一段也可直接128M作差
    //uint64 memorys_1 = PHY_SIZE - texts - rodatas;
    //其余需要映射的memory所占用的空间
    uint64 memorys = (uint64)PHY_END + (uint64)PA2VA_OFFSET - (uint64)_sdata;
    //大小表现一致
    //uint64 memorys_1 = (uint64)PHY_SIZE -  (uint64)sbis - ((uint64)_sdata - (uint64)_stext);


    //感觉XWRV在按时映射权限
    // mapping kernel text X|-|R|V
    //create_mapping(...);
    create_mapping(swapper_pg_dir, vms + sbis, phs + sbis, texts, 11);

    // mapping kernel rodata -|-|R|V
    //create_mapping(...);
    create_mapping(swapper_pg_dir, vms + sbis + texts, phs + sbis + texts, rodatas, 3);
    // mapping other memory -|W|R|V
    //create_mapping(...);
    create_mapping(swapper_pg_dir, vms + sbis + texts + rodatas, phs + sbis + texts + rodatas, memorys, 7);
    
    // set satp with swapper_pg_dir
    uint64 mode = 0x8000000000000000;
    //data/bss段已经被映射到虚拟地址处
    uint64 address = (uint64)swapper_pg_dir - PA2VA_OFFSET;
    address = address >> 12;
    uint64 temp = address | mode;

    __asm__ volatile(
        "csrw satp, %[temp]\n"
        :
        :[temp]"r"(temp)
    );

    // flush TLB
    asm volatile("sfence.vma zero, zero");
  
    // flush icache
    asm volatile("fence.i");
    return;
}


/**** 创建多级页表映射关系 *****/
/* 不要修改该接口的参数和返回值 */
create_mapping(uint64 *pgtbl, uint64 va, uint64 pa, uint64 sz, uint64 perm) {
    /*
    pgtbl 为根页表的基地址
    va, pa 为需要映射的虚拟地址、物理地址
    sz 为映射的大小，单位为字节
    perm 为映射的权限 (即页表项的低 8 位)

    创建多级页表的时候可以使用 kalloc() 来获取一页作为页表目录
    可以使用 V bit 来判断页表项是否存在
    */

    //2^12字节/页
    //需要处理不为页倍数的情况
    uint64 pnumber;
    if( sz % 4096 != 0)
        //向上取整
        pnumber = sz >> 12 + 1;
    else
        pnumber = sz >> 12;

    //将每一页填入列表之中
    for(uint64 i = 0; i < pnumber; i++)
    {
        //计算当前虚拟地址的实际位置
        uint64 now_va = va + i * PGSIZE;
        uint64 now_pa = pa + i * PGSIZE;
        
        //计算当前虚拟地址对映的vpn信息
        uint64 vpn2 = (now_va >> 30) & 0x1ff;
        uint64 vpn1 = (now_va >> 21) & 0x1ff;
        uint64 vpn0 = (now_va >> 12) & 0x1ff;

        //查看第一级页表是否存在
        uint64 pte_1 = pgtbl[vpn2];
        //不存在则需创建
        if((pte_1 & 1) == 0)
        {
            uint64 newadd_1 = kalloc();
            //printk("va or pa 2 %d\n",newadd_1);
            //check kalloc函数是否直接分配了虚拟地址
            //虚拟地址
            newadd_1 = newadd_1 - PA2VA_OFFSET;
            //层级搜索，感觉有bit V置1就可以
            pte_1 = ((newadd_1 & 0x00FFFFFFFFFFF000) >> 2) | 0x1;
            //更新根页表pte信息
            pgtbl[vpn2] = pte_1;
        }

        //从物理地址回到虚拟地址开始偏移
        uint64* pgtbl_1;
        pgtbl_1 = (uint64*) (((pte_1 & 0x003FFFFFFFFFFC00) << 2) + PA2VA_OFFSET);
        uint64 pte_2 = pgtbl_1[vpn1];

        if((pte_2 & 1) == 0)
        {
            uint64 newadd_2 = kalloc();
            //printk("va or pa 1 %d\n",newadd_2);
            //check kalloc函数是否直接分配了虚拟地址
            //正好与上相隔4096
            newadd_2 = newadd_2 - PA2VA_OFFSET;
            //层级搜索，感觉有bit V置1就可以
            pte_2 = ((newadd_2 & 0x00FFFFFFFFFFF000) >> 2) | 0x1;
            //更新根页表pte信息
            pgtbl_1[vpn1] = pte_2;
        }

        uint64* pgtbl_2;
        pgtbl_2 = (uint64*) (((pte_2 & 0x003FFFFFFFFFFC00) << 2) + PA2VA_OFFSET);
        uint64 pte_3;

        //层级搜索，感觉有bit V置1就可以
        pte_3 = ((now_pa & 0x00FFFFFFFFFFF000) >> 2) | perm;
        //更新根页表pte信息
        pgtbl_2[vpn0] = pte_3;

        //测试bug位置
        //32765最后一轮
        //31744可达
        /*if(i > 32176)
            printk(" now %d\n", i);*/
    }

}
