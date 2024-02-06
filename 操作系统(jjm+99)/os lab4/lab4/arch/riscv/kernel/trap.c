// trap.c 
#include "syscall.h"
struct pt_regs{
    //31个寄存器数值的存储，不包括0号寄存器
    uint64 reg_trap[31];
    //sepc寄存器存储
    uint64 sepc_trap;
    //sstatus寄存器存储（用于判断错误类型）
    uint64 sstatus_trap;  
};

void trap_handler(unsigned long scause, unsigned long sepc, struct pt_regs *regs) {
    // 通过 `scause` 判断trap类型
    // 如果是interrupt 判断是否是timer interrupt
    // 如果是timer interrupt 则打印输出相关信息, 并通过 `clock_set_next_event()` 设置下一次时钟中断
    // `clock_set_next_event()` 见 4.3.4 节
    // 其他interrupt / exception 可以直接忽略

    // YOUR CODE HERE
    //判断是否为interrupt
    //printk("scause:%lx\n",scause);
    if( (scause >> 63) == 1 )
    {
        //判断是否为timer interrupt
        if( (scause << 1 ) == 10 )
        {
            //printk("timer interrupt!\n");
            //printk("locate in : %u\n", sepc);//sepc
            //printk("[S] Supervisor Mode Timer Interrupt!\n");
            //printk("kernel is running!\n");
            clock_set_next_event();
            do_timer();
        }
    }
    else
    {
        //判断是不是系统调用的中断
        if(scause == 8)
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
        else
        {
            //printk("error!\n");
            ;
        }
    }
}