//arch/riscv/kernel/proc.c
#include "proc.h"
#include "mm.h"
#include "defs.h"
#include "rand.h"
#include "printk.h"
#include "test.h"
#include "string.h"
#include "elf.h"

//arch/riscv/kernel/proc.c

extern void __dummy();

struct task_struct* idle;           // idle process
struct task_struct* current;        // 指向当前运行线程的 `task_struct`
struct task_struct* task[NR_TASKS]; // 线程数组, 所有的线程都保存在此

/**
 * new content for unit test of 2023 OS lab2
*/
extern uint64 task_test_priority[]; // test_init 后, 用于初始化 task[i].priority 的数组
extern uint64 task_test_counter[];  // test_init 后, 用于初始化 task[i].counter  的数组
extern char _sramdisk[];//uapp段的起始地址
extern char _eramdisk[];//uapp段的起始地址
extern unsigned long  swapper_pg_dir[512];//获得内核页表


void do_mmap(struct task_struct *task, uint64_t addr, uint64_t length, uint64_t flags,
    uint64_t vm_content_offset_in_file, uint64_t vm_content_size_in_file);

static uint64_t load_program(struct task_struct* task) {
    //转化为对映的结构体
    Elf64_Ehdr* ehdr = (Elf64_Ehdr*)_sramdisk;

    //根据魔数判断是否为elf文件
    if(!(ehdr->e_ident[0] == 0x7f && ehdr->e_ident[1] == 'E' && ehdr->e_ident[2] == 'L' && ehdr->e_ident[3] == 'F')) 
    {
        printk("Not elf\n");
    }
    //计算phdr开始偏移位置
    uint64_t phdr_start = (uint64_t)ehdr + ehdr->e_phoff;
    //计算phdr段总个数
    int phdr_cnt = ehdr->e_phnum;
    //275个segment
    //printk("%d",phdr_cnt);
    Elf64_Phdr* phdr;
    int load_phdr_cnt = 0;
    //循环所有phdr段
    for (int i = 0; i < phdr_cnt; i++) 
    {
        //计算地址
        phdr = (Elf64_Phdr*)(phdr_start + sizeof(Elf64_Phdr) * i);
        //若为LOAD段
        if (phdr->p_type == PT_LOAD) 
        {
            // alloc space and copy content
            // do mapping     
            uint64 pa_num;
            //为段分配内存
            uint64 vir_add = (uint64)phdr->p_vaddr;
            //注意需要保持虚拟地址的offset，在实际中被右移12bit忽略
            uint64 offset = vir_add & 0xfff;
            pa_num = ((phdr->p_memsz + offset) + PGSIZE - 1) / PGSIZE;
            uint64 flags = 0;
            if(phdr->p_flags & PF_R)
                flags = flags | VM_R_MASK;
            if(phdr->p_flags & PF_W)
                flags = flags | VM_W_MASK;
            if(phdr->p_flags & PF_X)
                flags = flags | VM_X_MASK;

            do_mmap(task, vir_add, pa_num * PGSIZE, flags, phdr->p_offset, phdr->p_filesz);
            //for debug
            //printk("1%lx\n",(uint64)ehdr + phdr->p_offset);
            //printk("3%d\n",pa_num);


            /*
            //计算需要分配的内存页数
            pa_num = ((phdr->p_memsz + offset) + PGSIZE - 1) / PGSIZE;
            uint64 aoc_mem = (unsigned long*)alloc_pages(pa_num);
            //进行内容的赋值
            memcpy((void*)(aoc_mem + offset), (void *)((uint64)ehdr + phdr->p_offset), phdr->p_filesz); 
            //内存对映
            create_mapping(task->pgd, (uint64)phdr->p_vaddr, (uint64)aoc_mem - PA2VA_OFFSET, (uint64)phdr->p_memsz, 0x1F);
            */
        }
    }
    // allocate user stack and do mapping
    // code...
    //用户栈匿名页
    do_mmap(task, USER_END-PGSIZE, PGSIZE, VM_R_MASK | VM_W_MASK | VM_ANONYM, 0, 0);


    /*
    //申请空的用户态栈
    unsigned long* stack_map = (unsigned long*)alloc_page();
    //页表映射
    create_mapping(task->pgd, (uint64)USER_END - (uint64)PGSIZE, (uint64)stack_map - (uint64)PA2VA_OFFSET, (uint64)PGSIZE, 0x1F);
    // following code has been written for you
    // set user stack
    */

    // pc for the user program
    task->thread.sepc = (uint64)ehdr->e_entry;
    // sstatus bits set
    task->thread.sstatus = 0x40020;
    // user stack for user program
    task->thread.sscratch = (uint64)USER_END;
    
}

void task_init() {
    test_init(NR_TASKS);
    //先为每个进程开辟可以包含uapp内容的区域
    /* YOUR CODE HERE */
    // 1. 调用 kalloc() 为 idle 分配一个物理页
    idle = kalloc();
    //printk("%lx",idle);
    // 2. 设置 state 为 TASK_RUNNING;
    idle->state = TASK_RUNNING;
    // 3. 由于 idle 不参与调度 可以将其 counter / priority 设置为 0
    idle->counter = 0;
    idle->priority = 0;
    // 4. 设置 idle 的 pid 为 0
    idle->pid = 0;

    // 5. 将 current 和 task[0] 指向 idle
    current = idle;
    task[0] = idle;

    // 1. 参考 idle 的设置, 为 task[1] ~ task[NR_TASKS - 1] 进行初始化
    // 2. 其中每个线程的 state 为 TASK_RUNNING, 此外，为了单元测试的需要，counter 和 priority 进行如下赋值：
    //      task[i].counter  = task_test_counter[i];
    //      task[i].priority = task_test_priority[i];
    // 3. 为 task[1] ~ task[NR_TASKS - 1] 设置 `thread_struct` 中的 `ra` 和 `sp`,
    // 4. 其中 `ra` 设置为 __dummy （见 4.3.2）的地址,  `sp` 设置为 该线程申请的物理页的高地址
    //计算需要分配多少页
    uint64 num = _eramdisk - _sramdisk;
    uint64 page_num;
    if(num % PGSIZE != 0)
    {
        page_num = num / PGSIZE + 1;
    }
    else
    {
        page_num = num / PGSIZE;
    }
    //printk("%d\n",page_num);
    /* YOUR CODE HERE */
    //NR_TASKS
    //只初始化一个
    for(int i = 1; i < 2; i++)
    {
        // 1. 参考 idle 的设置, 为 task[1] ~ task[NR_TASKS - 1] 进行初始化
        task[i] = kalloc();
        task[i]->state = TASK_RUNNING;
        task[i]->counter  = task_test_counter[i];
        //初始时间片设定输出
        //printk("task[%d] counter %d\n", i, task[i]->counter);
        task[i]->priority = task_test_priority[i];
        //初始优先级设定输出
        //printk("task[%d] priority %d\n", i, task[i]->priority);
        //pid暂时设置
        task[i]->pid = i;
        //printk("SET [PID = %d COUNTER = %d]\n", task[i]->pid, task[i]->counter);

        //设置ra和sp
        //sp指向最末尾区域
        task[i]->thread.sp = (uint64)task[i] + PGSIZE;
        //ra设置为__dummy地址
        __asm__ volatile(
        "la t0, __dummy\n"
        "mv %[temp],t0\n"
        :[temp] "=r" (task[i]->thread.ra)
        );

        
        //lab4-4
        //建立独立的页表,先分配一页
        task[i]->pgd = (unsigned long*)alloc_page();
        //获得内核页表
        memcpy(task[i]->pgd, swapper_pg_dir, PGSIZE); 
        //4096没问题
        //printk("%d\n",PGSIZE);
        
        load_program(task[i]);

        /*
        //user_start
        task[i]->thread.sepc = USER_START;
        //SPP第8bit为0表示返回为用户模式
        //SPIE第5bit为1表示程序返回用户模式后可中断
        //SUM第18bit为1，表示内核态可以访问用户态信息
        task[i]->thread.sstatus = 0x40020;
        //设置为U-mode的sp数值（虚拟地址）
        task[i]->thread.sscratch = USER_END;

        //需要将uapp段拷贝至进程中
        unsigned long* memory_map = (unsigned long*)alloc_pages(page_num);
        //1908字节数没有问题
        //printk("%d\n",_eramdisk - _sramdisk);
        memcpy(memory_map, _sramdisk, _eramdisk - _sramdisk);
        //memcpy(memory_map, _sramdisk, PGSIZE);
        //页表映射
        //printk("va:%lx\n",USER_START);
        //printk("pgd %lx\n",task[i]->pgd[0]);
        create_mapping(task[i]->pgd, (uint64)USER_START, (uint64)memory_map - (uint64)PA2VA_OFFSET, (uint64)_eramdisk -(uint64) _sramdisk , 0x1F);
        //申请空的用户态栈
        unsigned long* stack_map = (unsigned long*)alloc_page();
        //页表映射
        create_mapping(task[i]->pgd, (uint64)USER_END - (uint64)PGSIZE, (uint64)stack_map - (uint64)PA2VA_OFFSET, (uint64)PGSIZE, 0x1F);
        */
    }
    for(int i = 2; i < NR_TASKS; i++)
    {
        task[i] = 0L;
    }
    printk("...proc_init done!\n");
}

// arch/riscv/kernel/proc.c
void dummy() {
    //uint64 temp;
    schedule_test();
    uint64 MOD = 1000000007;
    //累计运行时间记录
    uint64 auto_inc_local_var = 0;
    //上一轮current数值计数器
    int last_counter = -1;
    //uint64 last_counter = 0;
    while(1) {
        //线程刚调用或线程时间片减少1且当前剩余时间片 > 0 时，输出累计运行时间片信息
        if ((last_counter == -1 || current->counter != last_counter) && current->counter > 0) {
            //printk("%d %d\n",last_counter,current->counter);
            //为1自动减为0
            if(current->counter == 1){
                --(current->counter);   // forced the counter to be zero if this thread is going to be scheduled
            }                           // in case that the new counter is also 1, leading the information not printed.
            last_counter = current->counter;
            auto_inc_local_var = (auto_inc_local_var + 1) % MOD;
            //输出线程累计运行时间片个数信息
            //printk("[PID = %d] is running. auto_inc_local_var = %d\n", current->pid, auto_inc_local_var);
            printk("[PID = %d] is running! Thread space begin at %lx\n",current->pid, current);
        }
    }
}

extern void __switch_to(struct task_struct* prev, struct task_struct* next);

void switch_to(struct task_struct* next) {
    /* YOUR CODE HERE */
    //通过pid判断两个线程是否一致
    struct task_struct* temp = current;
    if(temp->pid != next->pid)
    {
       //printk("switch to [PID = %d counter = %d]\n", next->pid, next->counter);
       current = next;
       //current->counter = current->counter + 1;
       //汇编语言实现，其实指针并没有改向，prev不设置也没问题
       //printk("%lx\n",next->thread.sstatus);
       //printk("%lx\n",next->thread.sscratch);
        __switch_to(temp,next);
    }
        
}

void do_timer(void) {
    // 1. 如果当前线程是 idle 线程 直接进行调度
    // 2. 如果当前线程不是 idle 对当前线程的运行剩余时间减1 若剩余时间仍然大于0 则直接返回 否则进行调度
    if(current == idle)
    {
        printk("idle process is running!\n");
        printk("\n");
    }
        
    /* YOUR CODE HERE */
    if( (current == idle) || (current->counter == 0) )
    {
        //进行调度；
        schedule();
        //printk("schedule happen\n");
    }
    else
    {
        current->counter = current->counter - 1;
    }
    
}

void schedule(void) {
    /* YOUR CODE HERE */
    //保存最小counter与对映的进程位次
    uint64 nowmin = 0;
    int flag = 0;
    int number;
    //下一执行的进程
    struct task_struct* next;
    //SJF算法
    //遍历获得最短作业
    #ifdef SJF
        //NR_TASKS
        for(int i = 1; i < NR_TASKS; i++)
        {
            //若处于task_running状态以及剩余时间更少，存为切换目标
            //保证存为目标的task剩余时间片>0
            if(task[i] != 0L && task[i]->state == TASK_RUNNING && task[i]->counter > 0)
            {
                //第一次进入时，为最小变量附初值
                if(flag == 0)
                {
                    nowmin = task[i]->counter;
                    number = i;
                    flag = 1;
                }
                //后续进入，只能刷新最小变量
                else if(flag == 1)
                {
                    if(task[i]->counter < nowmin)
                    {
                        //记录最小变量数值及对映task序号
                        nowmin = task[i]->counter;
                        number = i;
                    }
                }
            }
        }
        //判断是否所有线程均执行完毕（时间片为0）
        if(nowmin != 0)
        {
            next = task[number];
            printk("SWITCH TO [PID = %d COUNTER = %d]\n", next->pid, next->counter);
            // printk("%lx\n",next);
            //进行线程转换
            switch_to(next);
        }
        else
        {
            //均执行完时间，则rand新counter数值
            for(int i = 1; i < NR_TASKS; i++)
            {
                if(task[i] != 0L)
                {
                    // task[i]->counter = rand();
                    task[i]->counter = rand();
                    printk("SET [PID = %d COUNTER = %d]\n", task[i]->pid, task[i]->counter);
                }
            }
            //printk("assign down\n");
            schedule();
        }
    #endif

    #ifdef PRIORITY
        //优先级算法（课本，仅比较priority）
        for(int i = 1; i < NR_TASKS; i++)
        {
            //若处于task_running状态以及剩余时间更长，存为切换目标
            //剩余时间对映优先级的高低
            if(task[i]->state == TASK_RUNNING && task[i]->counter > 0)
            {
                if(flag == 0)
                {
                    nowmin = task[i]->counter;
                    number = i;
                    flag = 1;
                }
                else if(flag == 1)
                {
                    if(task[i]->counter > nowmin)
                    {
                        nowmin = task[i]->counter;
                        number = i;
                    }
                }
            }
        }
        if(nowmin != 0)
        {
            next = task[number];
            //进行线程转换
            switch_to(next);
        }
        else
        {
            //均执行完时间，则rand新counter数值
            for(int i = 1; i < NR_TASKS; i++)
            {
                task[i]->counter = task[i]->priority;
                //printk("assign task[%d] %d counters.\n", i, task[i]->counter);
            }
            //printk("assign down\n");
            schedule();
        }
    #endif
}


void hello_text(void)
{
    printk("Hello text!\n");
}

void hello_rodata(void)
{
    printk("Hello rodata!\n");
}

uint64 getcur_id()
{
    return current->pid;
}


void do_mmap(struct task_struct *task, uint64_t addr, uint64_t length, uint64_t flags,
    uint64_t vm_content_offset_in_file, uint64_t vm_content_size_in_file)
{
    struct vm_area_struct* vma_0 = &(task->vmas[task->vma_cnt]);
    task->vma_cnt++;
    //用户程序设置
    vma_0->vm_content_offset_in_file = vm_content_offset_in_file;
    vma_0->vm_content_size_in_file = vm_content_size_in_file;
    vma_0->vm_start = addr;
    vma_0->vm_end = addr + length;
    vma_0->vm_flags = flags;
    // printk("1%lx\n",addr);

}


struct vm_area_struct *find_vma(struct task_struct *task, uint64_t addr)
{
    struct vm_area_struct * vma;
    for(int i = 0; i < task->vma_cnt; i++)
    {
        if(addr >= task->vmas[i].vm_start && addr < task->vmas[i].vm_end)
        {
            vma = &(task->vmas[i]);
            break;
        }
    }
    return vma;
}