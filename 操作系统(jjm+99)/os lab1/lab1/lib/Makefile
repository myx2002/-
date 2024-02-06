C_SRC       = $(sort $(wildcard *.c)) #记录所有.c文件，并进行排序
OBJ		    = $(patsubst %.c,%.o,$(C_SRC)) #把所有.c文件转换为.o文件

file = printk.o
all:$(OBJ) #生成为目标

#依赖于.c文件进行相应的编译
%.o:%.c
	${GCC} ${CFLAG} -c $< 

#删除所有的.o文件
clean:
	$(shell rm *.o 2>/dev/null)