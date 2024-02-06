    .section .text.entry
    .align 2
    .globl _traps 
_traps:
    # YOUR CODE HERE
    # -----------

    # 1. save 32 registers and sepc to stack
    //判断是否为内核态中断
    //若不为内核态中断，则进行寄存器数值的替换
    .global _jump
    csrr t1,  sscratch
    beq t1, zero, _jump
    mv t0, sp
    csrw sscratch, t0
    mv sp, t1

_jump:
    sd x2,   -8*35(sp)
    addi sp, sp, - 8*36
    sd x1,   0(sp)
    sd x3,   16(sp)
    sd x4,   24(sp)
    sd x5,   32(sp)
    sd x6,   40(sp)
    sd x7,   48(sp)
    sd x8,   56(sp)
    sd x9,   64(sp)
    sd x10,  72(sp)
    sd x11,  80(sp)
    sd x12,  88(sp)
    sd x13,  96(sp)
    sd x14,  104(sp)
    sd x15,  112(sp)
    sd x16,  120(sp)
    sd x17,  128(sp)
    sd x18,  136(sp)
    sd x19,  144(sp)
    sd x20,  152(sp)
    sd x21,  160(sp)
    sd x22,  168(sp)
    sd x23,  176(sp)
    sd x24,  184(sp)
    sd x25,  192(sp)
    sd x26,  200(sp)
    sd x27,  208(sp)
    sd x28,  216(sp)
    sd x29,  224(sp)
    sd x30,  232(sp)
    sd x31,  240(sp)
    csrr t0, sepc
    sd t0, 248(sp)
    csrr t0, sstatus
    sd t0, 256(sp)
    csrr t0, stval
    sd t0, 264(sp)
    csrr t0, sscratch
    sd t0, 272(sp)
    csrr t0, scause
    sd t0, 280(sp)

    # -----------

    # 2. call trap_handler

    csrr a0, scause
    csrr a1, sepc
    mv a2, sp
    jal x1, trap_handler

    .global __ret_from_fork
__ret_from_fork:
    #修改修改
    #利用 sp 从栈中恢复出寄存器的值 
    #sret

    # -----------

    # 3. restore sepc and 32 registers (x2(sp) should be restore last) from stack
    ld t0, 248(sp)
    csrw sepc, t0
    ld t0, 256(sp)
    csrw sstatus, t0
    ld t0, 264(sp)
    csrw stval, t0
    ld t0, 272(sp)
    csrw sscratch, t0
    ld t0, 280(sp)
    csrw scause, t0
    ld x1,   0(sp)
    ld x3,   16(sp)
    ld x4,   24(sp)
    ld x5,   32(sp)
    ld x6,   40(sp)
    ld x7,   48(sp)
    ld x8,   56(sp)
    ld x9,   64(sp)
    ld x10,  72(sp)
    ld x11,  80(sp)
    ld x12,  88(sp)
    ld x13,  96(sp)
    ld x14,  104(sp)
    ld x15,  112(sp)
    ld x16,  120(sp)
    ld x17,  128(sp)
    ld x18,  136(sp)
    ld x19,  144(sp)
    ld x20,  152(sp)
    ld x21,  160(sp)
    ld x22,  168(sp)
    ld x23,  176(sp)
    ld x24,  184(sp)
    ld x25,  192(sp)
    ld x26,  200(sp)
    ld x27,  208(sp)
    ld x28,  216(sp)
    ld x29,  224(sp)
    ld x30,  232(sp)
    ld x31,  240(sp)
    addi sp, sp, 8*36
    # ld x2,   -8*35(sp)

    //__trap函数运行结束后，也需要介导寄存器数值的交换
    .global _jump_o
    csrr t1, sscratch
    beq t1, zero, _jump_o
    mv t0, sp
    csrw sscratch, t0
    mv sp, t1
    # -----------
_jump_o:
    # 4. return from trap
    sret

    # ----------- 

    .global __dummy
__dummy:
    # YOUR CODE HERE
    #交换sp sscratch寄存器的值
    //从内核态内核栈转换为用户态用户核
    mv t0,sp 
    csrr t1, sscratch
    csrw sscratch, t0
    mv sp, t1
    #la t0, dummy
    #csrw sepc,t0
    sret

    .globl __switch_to
__switch_to:
    # save state to prev process
    # YOUR CODE HERE
    //task_struct数据结构的偏移量为40Byte
    //lab4修改
    li t0, 40
    //a0,a1作为参数接受者，a0应该表示prev
    add t1, a0, t0
    //根据偏移量存储寄存器信息至struct
    sd ra, 0(t1)
    sd sp, 8*1(t1)
    sd s0, 8*2(t1)
    sd s1, 8*3(t1)
    sd s2, 8*4(t1)
    sd s3, 8*5(t1)
    sd s4, 8*6(t1)
    sd s5, 8*7(t1)
    sd s6, 8*8(t1)
    sd s7, 8*9(t1)
    sd s8, 8*10(t1)
    sd s9, 8*11(t1)
    sd s10, 8*12(t1)
    sd s11, 8*13(t1)
    #lab4
    #需要将sepc，sstatus，sscratch寄存器的数值保存与栈
    csrr t2, sepc
    sd t2, 8*14(t1)
    csrr t2, sstatus
    sd t2, 8*15(t1)
    csrr t2, sscratch
    sd t2, 8*16(t1)

    #基于satp寄存器进行页表的替换
    #csrw satp, t2
    #sd t2, 8*17(t1)
    
    # restore state from next process
    add t2, a1, t0
    //根据偏移量读取struct信息至寄存器
    ld ra, 0(t2)
    ld sp, 8*1(t2)
    ld s0, 8*2(t2)
    ld s1, 8*3(t2)
    ld s2, 8*4(t2)
    ld s3, 8*5(t2)
    ld s4, 8*6(t2)
    ld s5, 8*7(t2)
    ld s6, 8*8(t2)
    ld s7, 8*9(t2)
    ld s8, 8*10(t2)
    ld s9, 8*11(t2)
    ld s10, 8*12(t2)
    ld s11, 8*13(t2)
    #lab4
    //取出栈中sepc，sstatus，sscratch寄存器的数值
    ld t1, 8*14(t2)
    csrw sepc, t1
    ld t1, 8*15(t2)
    csrw sstatus, t1
    ld t1, 8*16(t2)
    csrw sscratch, t1

    #使用新的satp
    //基于结构中pgd进行页表物理地址的存储
    ld t1, 8*17(t2)
    li t0, 0xffffffdf80000000
    sub t1, t1, t0
    srli t1, t1, 12
    li t0, 0x8000000000000000
    or t0, t0, t1
    csrw satp, t0
    //设置新页表后，刷新TLB与icache
    sfence.vma zero, zero
    fence.i
    //vma.fence

    # YOUR CODE HERE

    ret




