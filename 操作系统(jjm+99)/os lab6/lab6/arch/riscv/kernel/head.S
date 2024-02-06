#define PA2VA_OFFSET (0xffffffe000000000 - 0x0000000080000000)

.extern start_kernel
.extern early_pgtbl
.extern setup_vm
.extern relocate
.extern mm_init
.extern setup_vm_final
.extern task_init
.extern hello_text
.extern hello_rodata


    .section .text.init
    .globl _start
_start:
    //la sp, boot_stack_top # boot_stack_top的地址给sp（riscv中用于堆栈操作）
    //直接使用立即数可能会在后期出现问题，可以使用#include"def.h"进行修改
    # la sp, 0x80205
    # slli sp, sp, 12
    la sp, boot_stack_top
    li t0, 0xffffffdf80000000
    sub sp, sp, t0 
    call setup_vm
    call relocate

    //！！！还是要注意一下
    //la sp, boot_stack_top # boot_stack_top的地址给sp（riscv中用于堆栈操作）
    //使用mm_init函数进行内存初始化
    .global _new_try
_new_try: 
    call mm_init
    //call hello_text
    //在内存初始化之后，应该要在其他函数运行之前
    call setup_vm_final
    //对线程进行初始化
    call task_init

    //对线程进行初始化
    //call task_init

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
    //不受中断影响
    //csrw sstatus, t1

    j start_kernel # 跳转到对映标签函数处，以字节为单位（注意使用的opcode）

relocate:
    # set ra = ra + PA2VA_OFFSET
    # set sp = sp + PA2VA_OFFSET (If you have set the sp before)
    la t0, set_sepc
    csrw stvec, t0

    //直接作差可能会有一定的错误
    li t0, 0xffffffdf80000000
    add ra, ra, t0
    add sp, sp, t0


    # set satp with early_pgtbl
    //获得页表的地址
    la t1, early_pgtbl
    sub t1, t1, t0
    srli t1, t1, 12
    //调节mode数值
    li t0, 0x8000000000000000
    //设置satp寄存器
    or t0, t0, t1
    csrw satp, t0

    //测试新方法
    
    # flush tlb
    #刷新TLB
    /*sfence.vma zero, zero

    # flush icache
    #刷新 icache
    fence.i

    ret*/
    
    .global set_sepc
set_sepc:
    sfence.vma zero, zero
    fence.i
    la t0, _new_try
    csrw sepc, t0
    sret
    

    .section .rodata.try
    .global hello
hello:
    call hello_rodata

    .section .bss.stack
    .globl boot_stack
boot_stack:
    .space 4096 #use type to check，bytes

    .globl boot_stack_top
boot_stack_top: