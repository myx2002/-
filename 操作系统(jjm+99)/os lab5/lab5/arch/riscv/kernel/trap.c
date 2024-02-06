// trap.c 
#include "syscall.h"
#include "defs.h"
#include "proc.h"

extern struct task_struct* current;
extern char _sramdisk[];//uapp段的起始地址

struct pt_regs{
    //31个寄存器数值的存储，不包括0号寄存器
    uint64 reg_trap[31];
    //sepc寄存器存储
    uint64 sepc_trap;
    //sstatus寄存器存储（用于判断错误类型）
    uint64 sstatus_trap;  
    //page fault记录触发指令
    uint64 stval;
    uint64 sscratch;
    uint64 scause;
};
void do_page_fault(struct pt_regs *regs);

void trap_handler(unsigned long scause, unsigned long sepc, struct pt_regs *regs) {
    // 通过 `scause` 判断trap类型
    // 如果是interrupt 判断是否是timer interrupt
    // 如果是timer interrupt 则打印输出相关信息, 并通过 `clock_set_next_event()` 设置下一次时钟中断
    // `clock_set_next_event()` 见 4.3.4 节
    // 其他interrupt / exception 可以直接忽略

    // YOUR CODE HERE
    //判断是否为interrupt
    //printk("scause:%lx\n",scause);
    if (scause == 0x8000000000000005) 
    {
        //printk("Time interrupt!\n");
        clock_set_next_event();
        do_timer();
    }
    //判断是不是系统调用的中断
    else if(scause == 8)
    {
        //获得a7的数值
        uint64 sys_num = regs->reg_trap[16];
        //printk("%d\n",sys_num);
        //根据a7的数值查看是何类系统调用
        if(sys_num == SYS_GETPID)
        {
            //输出当前运行用户进程的pid
            regs->reg_trap[9] = sys_getpid();
            //printk("%d\n",regs->reg_trap[9]);
        }
        else if(sys_num == SYS_WRITE)
        {
            //打印需要输出的内容
            regs->reg_trap[9] = sys_write(regs->reg_trap[9], regs->reg_trap[10], regs->reg_trap[11]);
        }
        //手动调整sepc返回的指令位置
         regs->sepc_trap += 4;
    }
    else if(scause == 12 || scause == 13 || scause == 15)
    {
        //Instruction Page Fault
        //printk("[S] Unhandled trap, ");
        //printk("scause: %lx, ", scause);
        //printk("stval: %lx, ", regs->stval);
        //printk("sepc: %lx\n", regs->sepc_trap);
        printk("Page fault!\n");
        printk("scause: %lx, ", scause);
        //输出剩余关键寄存器信息
        printk("stval: %lx, ", regs->stval);
        printk("sepc: %lx\n", regs->sepc_trap);
        do_page_fault(regs);
    }
    else
    {
        printk("[S] Unhandled trap, ");
        //输出错误类型（基于scause判断）
        printk("scause: %lx, ", scause);
        //输出剩余关键寄存器信息
        printk("stval: %lx, ", regs->stval);
        printk("sepc: %lx\n", regs->sepc_trap);
        //防止输出抽搐
        while(1);
    }
}

void do_page_fault(struct pt_regs *regs) 
{
    /*
     1. 通过 stval 获得访问出错的虚拟内存地址（Bad Address）
     2. 通过 find_vma() 查找 Bad Address 是否在某个 vma 中
     3. 分配一个页，将这个页映射到对应的用户地址空间
     4. 通过 (vma->vm_flags & VM_ANONYM) 获得当前的 VMA 是否是匿名空间
     5. 根据 VMA 匿名与否决定将新的页清零或是拷贝 uapp 中的内容
    */
    //通过 stval 获得访问出错的虚拟内存地址（Bad Address）
    uint64 bad_address = regs->stval;
    struct task_struct* task = current;
    //寻找对映的vma结构
    struct vm_area_struct* vma = find_vma(task, bad_address);

    //计算相应的页内offset
    uint64 offset = bad_address & 0xfff;
    //计算文件起始地址
    uint64 from_add = (uint64)_sramdisk + vma->vm_content_offset_in_file;

    //若为匿名区域，则分配stack
    if(vma->vm_flags & VM_ANONYM)
    {
        //分配页面
        uint64 new_mem = (unsigned long*)alloc_page();
        //清空
        memset(new_mem, 0, PGSIZE);
        //产生映射
        create_mapping(task->pgd, (uint64)bad_address - offset, (uint64)new_mem - PA2VA_OFFSET, (uint64)PGSIZE, 0x1F);
    }
    else
    { 
        //计算段页面需要个数
        uint64 page_num = (vma->vm_end - vma->vm_start) / PGSIZE;
        //分配页
        uint64 new_mem = (unsigned long*)alloc_pages(page_num);
        //计算起始地址offset
        uint64 fi_off = (uint64)vma->vm_start & 0xfff;
        //进行内容的赋值
        memcpy((void*)(new_mem + fi_off), (void *)(from_add), vma->vm_content_size_in_file); 
        //内存对映
        create_mapping(task->pgd, (uint64)vma->vm_start, (uint64)new_mem - PA2VA_OFFSET, (uint64)(vma->vm_end - vma->vm_start), 0x1F);
    }
}