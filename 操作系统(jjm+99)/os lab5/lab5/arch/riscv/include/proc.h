// arch/riscv/include/proc.h

#include "types.h"

#define NR_TASKS  (1 + 3) // 用于控制 最大线程数量 （idle 线程 + 31 内核线程）

#define TASK_RUNNING    0 // 为了简化实验, 所有的线程都只有一种状态

#define PRIORITY_MIN 1
#define PRIORITY_MAX 10

#define VM_X_MASK         0x0000000000000008
#define VM_W_MASK         0x0000000000000004
#define VM_R_MASK         0x0000000000000002
#define VM_ANONYM         0x0000000000000001
//struct task_struct* current;        // 指向当前运行线程的 `task_struct`

struct vm_area_struct {
    /* VMA 对应的用户态虚拟地址的开始   */
    uint64 vm_start;      
    /* VMA 对应的用户态虚拟地址的结束   */    
    uint64 vm_end;  
    /* VMA 对应的 flags */          
    uint64 vm_flags;          

    /* uint64_t file_offset_on_disk */  /* 原本需要记录对应的文件在磁盘上的位置，
                                但是我们只有一个文件 uapp，所以暂时不需要记录 */

    uint64 vm_content_offset_in_file;                /* 如果对应了一个文件，
                        那么这块 VMA 起始地址对应的文件内容相对文件起始位置的偏移量，
                                          也就是 ELF 中各段的 p_offset 值 */

    uint64 vm_content_size_in_file;                
    //取值不一样，一方面是实际使用，一方面是总大小
};

/* 用于记录 `线程` 的 `内核栈与用户栈指针` */
/* (lab2 中无需考虑, 在这里引入是为了之后实验的使用) */
typedef unsigned long* pagetable_t;
struct thread_info {
    uint64 kernel_sp;
    uint64 user_sp;
};

/* 线程状态段数据结构 */
struct thread_struct {
    uint64 ra; //返回地址寄存器保存
    uint64 sp; //栈指针寄存器保存
    uint64 s[12]; //必要保存寄存器
    //这里数据没有考虑堆栈保存，是否需要保存
    //对映sepc寄存器，用于确定用户的返回值
    uint64 sepc;
    //确定用户返回状态与控制信息
    uint64 sstatus;
    //用于记录用户栈，用于与内核栈进行替换
    uint64 sscratch; 
};

/* 线程数据结构 */
struct task_struct {
    //注意由结构变化为指针（无用）
    struct thread_info* thread_info;
    //进程状态
    uint64 state;    
    //时间片剩余
    uint64 counter; 
    //优先级
    uint64 priority; 
    //进程ID
    uint64 pid;    

    struct thread_struct thread;
    pagetable_t pgd;

    /*数组里的元素的数量 */
    uint64 vma_cnt;     
    //柔性数组，只允许结构体的最后一个参数为此                  
    struct vm_area_struct vmas[0];   
};

/* 线程初始化 创建 NR_TASKS 个线程 */
void task_init();

/* 在时钟中断处理中被调用 用于判断是否需要进行调度 */
void do_timer();

/* 调度程序 选择出下一个运行的线程 */
void schedule();

/* 线程切换入口函数*/
void switch_to(struct task_struct* next);

/* dummy funciton: 一个循环程序, 循环输出自己的 pid 以及一个自增的局部变量 */
void dummy();

/*获得current的pid*/
uint64 getcur_id();
