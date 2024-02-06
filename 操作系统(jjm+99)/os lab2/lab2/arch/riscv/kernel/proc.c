//arch/riscv/kernel/proc.c
#include "proc.h"
#include "mm.h"
#include "defs.h"
#include "rand.h"
#include "printk.h"
#include "test.h"

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

void task_init() {
    test_init(NR_TASKS);
    /* YOUR CODE HERE */
    // 1. 调用 kalloc() 为 idle 分配一个物理页
    idle = kalloc();
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

    /* YOUR CODE HERE */
    for(int i=1; i<NR_TASKS; i++)
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
        printk("SET [PID = %d COUNTER = %d]\n", task[i]->pid, task[i]->counter);

        //设置ra和sp
        //sp指向最末尾区域
        task[i]->thread.sp = (uint64)task[i] + PGSIZE;
        //ra设置为__dummy地址
        __asm__ volatile(
        "la t0, __dummy\n"
        "mv %[temp],t0\n"
        :[temp] "=r" (task[i]->thread.ra)
        );
    }

    printk("...proc_init done!\n");
}

// arch/riscv/kernel/proc.c
void dummy() {
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
            printk("[PID = %d] is running. auto_inc_local_var = %d\n", current->pid, auto_inc_local_var);
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
       printk("switch to [PID = %d counter = %d]\n", next->pid, next->counter);
       current = next;
       //current->counter = current->counter + 1;
       //汇编语言实现，其实指针并没有改向，prev不设置也没问题
        __switch_to(temp,next);
    }
        
}

void do_timer(void) {
    // 1. 如果当前线程是 idle 线程 直接进行调度
    // 2. 如果当前线程不是 idle 对当前线程的运行剩余时间减1 若剩余时间仍然大于0 则直接返回 否则进行调度

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
        for(int i = 1; i < NR_TASKS; i++)
        {
            //若处于task_running状态以及剩余时间更少，存为切换目标
            //保证存为目标的task剩余时间片>0
            if(task[i]->state == TASK_RUNNING && task[i]->counter > 0)
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
            //进行线程转换
            switch_to(next);
        }
        else
        {
            //均执行完时间，则rand新counter数值
            for(int i = 1; i < NR_TASKS; i++)
            {
                task[i]->counter = rand();
                printk("SET [PID = %d COUNTER = %d]\n", task[i]->pid, task[i]->counter);
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
                printk("assign task[%d] %d counters.\n", i, task[i]->counter);
            }
            printk("assign down\n");
            schedule();
        }
    #endif
}
