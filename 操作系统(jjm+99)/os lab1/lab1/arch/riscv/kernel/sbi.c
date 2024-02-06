#include "types.h"
#include "sbi.h"

//unsigned long int ---->unit64
struct sbiret sbi_ecall(int ext, int fid, uint64 arg0,
			            uint64 arg1, uint64 arg2,
			            uint64 arg3, uint64 arg4,
			            uint64 arg5) 
{
    // unimplemented 
	struct sbiret ret; //先定义一特定的数据结构用于接受最总结果
	__asm__ volatile(

		"mv a7 , %[ext]\n"	//汇编语言将对映变量数据存入对映寄存器中
		"mv a6 , %[fid]\n"
		"mv a0 , %[arg0]\n"
		"mv a1 , %[arg1]\n"
		"mv a2 , %[arg2]\n"
		"mv a3 , %[arg3]\n"
		"mv a4 , %[arg4]\n"
		"mv a5 , %[arg5]\n"
		"ecall\n"			//调用ecall
		"mv %[error], a0\n"	//将得到的结果放回特定变量之中
		"mv %[value], a1\n"
		: [error] "=r" (ret.error),[value] "=r" (ret.value)
		: [arg0] "r" (arg0),[arg1] "r" (arg1),[arg2] "r" (arg2),[arg3] "r" (arg3),[arg4] "r" (arg4),[arg5] "r" (arg5),[ext] "r" (ext),[fid] "r" (fid)
	);
	return ret;
}
