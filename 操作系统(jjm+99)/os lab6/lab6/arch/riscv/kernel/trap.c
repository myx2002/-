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
    uint64 stval;
    uint64 sscratch;
    uint64 scause;
};
void do_page_fault(struct pt_regs *regs);
uint64 sys_clone(struct pt_regs *regs);

void trap_handler(unsigned long scause, unsigned long sepc, struct pt_regs *regs) {
    // 通过 `scause` 判断trap类型
    // 如果是interrupt 判断是否是timer interrupt
    // 如果是timer interrupt 则打印输出相关信息, 并通过 `clock_set_next_event()` 设置下一次时钟中断
    // `clock_set_next_event()` 见 4.3.4 节
    // 其他interrupt / exception 可以直接忽略

    // YOUR CODE HERE
    //判断是否为interrupt
    //printk("scause:%lx\n",scause);
    // printk("[S] Unhandled trap, ");
    // printk("scause: %lx, ", scause);
    // printk("stval: %lx, ", regs->stval);
    // printk("sepc: %lx\n", regs->sepc_trap);
    // printk("current:%lx %lx\n",current,regs->reg_trap[0]);
    // printk("current:%lx %lx\n",current,regs->sepc_trap);
    if (scause == 0x8000000000000005) 
    {
        //printk("Time interrupt!\n");
        clock_set_next_event();
        // printk("%lx: ",current);
        // printk("%lx\n",regs->sepc_trap);
        do_timer();
        __asm__ volatile(
        "csrr t0, sepc\n"
        "mv %[temp],t0\n"
        :[temp] "=r" (regs->sepc_trap)
        );
        // uint64 sp;
        // __asm__ volatile(
        // "mv %[temp],ra\n"
        // :[temp] "=r" (sp)
        // );
        // printk("current:%lx %lx\n",current,sp);
        // printk("current:%lx %lx\n",current,regs->reg_trap[2]);
        // printk("current:%lx %lx\n",current,regs->reg_trap[0]);
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
            regs->sepc_trap += 4;
        }
        else if(sys_num == SYS_WRITE)
        {
            //打印需要输出的内容
            regs->reg_trap[9] = sys_write(regs->reg_trap[9], regs->reg_trap[10], regs->reg_trap[11]);
            regs->sepc_trap += 4;
        }
        else if(sys_num == SYS_CLONE)
        {
            
            //fork对映的系统调用
            // printk("current: %lx\n",current);
            // printk("%lx\n",regs->sepc_trap);
            // printk("current:%lx %lx\n",current,regs->reg_trap[0]);
            // printk("current:%lx %lx\n",current,regs->sepc_trap);
            regs->reg_trap[9] = sys_clone(regs);
            //不知道要不要
            regs->sepc_trap += 4;
        }
    }
    else if(scause == 12 || scause == 13 || scause == 15)
    {
        
        //Instruction Page Fault
        //printk("[S] Unhandled trap, ");
        //printk("scause: %lx, ", scause);
        //printk("stval: %lx, ", regs->stval);
        //printk("sepc: %lx\n", regs->sepc_trap);
        do_page_fault(regs);
    }
    else
    {
        printk("[S] Unhandled trap, ");
        printk("scause: %lx, ", scause);
        printk("stval: %lx, ", regs->stval);
        printk("sepc: %lx\n", regs->sepc_trap);
        while(1);
    }
    //ra介导返回
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
    uint64 bad_address = regs->stval;
    struct task_struct* task = current;
    struct vm_area_struct* vma = find_vma(task, bad_address);

    uint64 offset = bad_address & 0xfff;
    // uint64 from_add = (uint64)_sramdisk + vma->vm_content_offset_in_file;

    if(vma->vm_flags & VM_ANONYM)
    {
        uint64 new_mem = (unsigned long*)alloc_page();
        // printk("%lx\n",new_mem);
        memset(new_mem, 0, PGSIZE);
        create_mapping(task->pgd, (uint64)bad_address - offset, (uint64)new_mem - PA2VA_OFFSET, (uint64)PGSIZE, 0x1F);
        // printk("%lx\n",bad_address - offset);
    }
    else
    { 
        uint64 from_add = (uint64)_sramdisk + vma->vm_content_offset_in_file;
        uint64 page_num = (vma->vm_end - vma->vm_start) / PGSIZE;
        uint64 new_mem = (unsigned long*)alloc_pages(page_num);
        // printk("%lx\n",new_mem);
        uint64 fi_off = (uint64)vma->vm_start & 0xfff;
        //进行内容的赋值
        // printk("%lx",from_add);
        memcpy((void*)(new_mem + fi_off), (void *)(from_add), vma->vm_content_size_in_file); 
        //内存对映
        create_mapping(task->pgd, (uint64)vma->vm_start, (uint64)new_mem - PA2VA_OFFSET, (uint64)(vma->vm_end - vma->vm_start), 0x1F);
        // printk("%lx\n",from_add + PGSIZE * 2);
        // printk("%lx\n",bad_address + PA2VA_OFFSET);
        // printk("%lx\n",from_add);
        // while(1);
    }
}

extern struct task_struct* task[NR_TASKS]; 
extern uint64 task_test_counter[];
extern uint64 task_test_priority[];
extern unsigned long  swapper_pg_dir[512];
uint64 sys_clone(struct pt_regs *regs) 
{
    
    //  1. 参考 task_init 创建一个新的 task，将的 parent task 的整个页复制到新创建的 
    //     task_struct 页上(这一步复制了哪些东西?）。将 thread.ra 设置为 
    //     __ret_from_fork，并正确设置 thread.sp
    //     (仔细想想，这个应该设置成什么值?可以根据 child task 的返回路径来倒推)
    uint64 i;
    //查找空闲的task
    for(i = 2; i < NR_TASKS; i++)
    {
        if(task[i] == 0L)
            break;
    }
    //若已没有空闲的task，则返回分配错误信息
    if(i >= NR_TASKS)
    {
        printk("Failed to allocate new page!\n");
        return -1;
    }
    //为task分配页空间
    task[i] = kalloc();
    uint64 parent_start =  current;
    //赋值父进程空间信息
    memcpy((void*)task[i], (void*)parent_start, PGSIZE);

    //单独pid设置
    task[i]->pid = i;

    //设置ra和sp
    task[i]->thread.sp = (uint64)task[i] + PGSIZE - 8 * 36;
    //将ra设置为__ret_from_fork
    __asm__ volatile(
    "la t0, __ret_from_fork\n"
    "mv %[temp],t0\n"
    :[temp] "=r" (task[i]->thread.ra)
    );

    //  2. 利用参数 regs 来计算出 child task 的对应的 pt_regs 的地址，
    //     并将其中的 a0, sp, sepc 设置成正确的值(为什么还要设置 sp?)
    //设置用户态栈指针
    task[i]->thread.sscratch = USER_END;
    struct pt_regs *sub_regs = (uint64)task[i]->thread.sp;
    //子进程对于fork函数返回值为0
    sub_regs->reg_trap[9] = 0;
    //返回指令为fork后一条
    sub_regs->sepc_trap = regs->sepc_trap + 4;

    //复制页表
    task[i]->pgd = (unsigned long*)alloc_page();
    memcpy(task[i]->pgd, swapper_pg_dir, PGSIZE);

    //  3. 为 child task 申请 user stack，并将 parent task 的 user stack 
    //     数据复制到其中。 (既然 user stack 也在 vma 中，这一步也可以直接在 5 中做，无需特殊处理)
    //在第五部分完成

    //  4. 为 child task 分配一个根页表，并仿照 setup_vm_final 来创建内核空间的映射
    //在前已完成

    //  5. 根据 parent task 的页表和 vma 来分配并拷贝 child task 在用户态会用到的内存
    int flag;
    //遍历父进程的vma结构
    for(int j = 0; j < current->vma_cnt; j++)
    {
        //对于每一vma结构，查询其start虚拟地址
        uint64 now_va = current->vmas[j].vm_start;
        flag = 0;
        //进行虚拟地址到物理地址的映射
        uint64 vpn2 = (now_va >> 30) & 0x1ff;
        uint64 vpn1 = (now_va >> 21) & 0x1ff;
        uint64 vpn0 = (now_va >> 12) & 0x1ff;

        //最外层页表查询
        uint64 pte_1 = current->pgd[vpn2];
        uint64 pte_2;
        uint64 pte_3;
        //若已存在
        if((pte_1 & 1) != 0)
        {
            uint64* pgtbl_1;
            //第二层页表查询
            pgtbl_1 = (uint64*) (((pte_1 & 0x003FFFFFFFFFFC00) << 2) + PA2VA_OFFSET);
            pte_2 = pgtbl_1[vpn1];
            //若已存在
            if((pte_2 & 1) != 0)
            {
                uint64* pgtbl_2;
                //第三层页表查询
                pgtbl_2 = (uint64*) (((pte_2 & 0x003FFFFFFFFFFC00) << 2) + PA2VA_OFFSET);
                pte_3 = pgtbl_2[vpn0];
                if((pte_3 & 1) != 0)
                    //设置已寻得物理地址的指示信息
                    flag = 1;
                else
                    continue;
            }
            else
                continue;
        }
        else
            continue;

        //判断为代码段还是栈空间
        if(flag && j != current->vma_cnt -1)
        {
            //计算虚拟空间大小，并分配相应大小的页面
            uint64 se_size = (uint64)(current->vmas[j].vm_end - current->vmas[j].vm_start);
            uint64 page_num = se_size / PGSIZE;
            uint64 new_mem = (unsigned long*)alloc_pages(page_num);
            //计算实际的offset，使页面内byte位置保持一致
            uint64 fi_off = (uint64)current->vmas[j].vm_start & 0xfff;
            uint64 from_add = ((pte_3 & 0x003FFFFFFFFFFC00) << 2) + PA2VA_OFFSET;
            //进行内容的复制与映射
            memcpy((void*)(new_mem + fi_off), (void *)(from_add + fi_off), se_size); 
            create_mapping(task[i]->pgd, current->vmas[j].vm_start, (uint64)new_mem - PA2VA_OFFSET, se_size, 0x1F);        
            // printk("1");   
        }
        else if(flag && j == current->vma_cnt -1)
        {
            //若为栈空间直接分配页面并进行复制
            uint64 new_stack = (unsigned long*)alloc_page();
            //计算获得真实的物理地址
            uint64 pte_final = (pte_3 & 0x003FFFFFFFFFFC00) << 2;
            //进行内容的复制与映射
            memcpy(new_stack, pte_final + PA2VA_OFFSET, PGSIZE);
            create_mapping(task[i]->pgd, (uint64)USER_END - PGSIZE, (uint64)new_stack - PA2VA_OFFSET, (uint64)PGSIZE, 0x1F);
        }
    }
    //  6. 返回子 task 的 pid
    return task[i]->pid;
}