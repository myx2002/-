.extern start_kernel

    .section .text.init
    .globl _start
_start:
    # ------------------
    # - your code here -
    # ------------------

    la sp, boot_stack_top # boot_stack_top的地址给sp（riscv中用于堆栈操作）
    //使用mm_init函数进行内存初始化
    call mm_init
    //对线程进行初始化
    call task_init

    # set stvec = _traps
    #末尾默认为00---->direct模式
    la t0, _traps
    csrw stvec, t0

    # set sie[STIE] = 1, STIE位于sie寄存器中的第五位
    csrr t0, sie
    li t1, 0x20 
    or t1, t1, t0
    csrw sie, t1

    # set first time interrupt
    rdtime t0
    li t1, 1000000
    add a0, t1, t0
    li a7, 0x00
    li a6, 0x0
    ecall

    # set sstatus[SIE] = 1, SIE位于sstatus寄存器中的第1位
    csrr t0, sstatus
    li t1, 0x2
    or t1, t1, t0
    csrw sstatus, t1

    j start_kernel # 跳转到对映标签函数处，以字节为单位（注意使用的opcode）

    .section .bss.stack
    .globl boot_stack
boot_stack:
    .space 4096 #use type to check，bytes

    .globl boot_stack_top
boot_stack_top: