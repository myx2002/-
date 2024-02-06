// trap.c 

void trap_handler(unsigned long scause, unsigned long sepc) {
    // 通过 `scause` 判断trap类型
    // 如果是interrupt 判断是否是timer interrupt
    // 如果是timer interrupt 则打印输出相关信息, 并通过 `clock_set_next_event()` 设置下一次时钟中断
    // `clock_set_next_event()` 见 4.3.4 节
    // 其他interrupt / exception 可以直接忽略

    // YOUR CODE HERE
    //判断是否为interrupt
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


}