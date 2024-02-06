import random
import time
random.seed(time.time())

#输入生成总个数，总宽度，大矩形生成因子，生成文件名
total_number = int(input("input total number："))
total_width = int(input("input total width："))
rate = float(input("input the rate: "))
name = input("input the txt_name: ")
record = total_number - 1;
width = []
height = []

#计算大小矩形相对生成个数
number = int((total_number + 1) * rate)
normal = total_number + 1 - number
for i in range(1,normal + 1):
    width.append(random.randint(1,11))
for i in range(1,number):
    width.append(random.randint(1,total_width))
#随机化宽度序列
random.shuffle(width)

for i in range(1,normal + 1):
    height.append(random.randint(1,11))
for i in range(1,number):
    height.append(random.randint(1,total_width))
#随机化高度序列
random.shuffle(height)

#合并宽度与高度列表，方便关联排序
order_list = zip(width,height)
#zip函数后，将结果转变为list
order_list = list(order_list)

#part1
file = open(name+"_random.txt",'w')
file.write(str(total_number) + "\n")
file.write(str(total_width) + "\n")
#指定格式输出随机序列
for i in range(1,total_number):
    file.write(str(i) + " " + str(width[i - 1]) + " " + str(height[i-1]) + "\n")
file.write(str(total_number) + " " + str(width[record]) + " " + str(height[record]))
file.close()

#part2
#对宽度进行从小到大排序
sort_result = sorted(order_list,key=lambda x:x[0])
i = 0
for x in sort_result:
    width[i],height[i] = x
    i = i + 1

file = open(name+"_weight_stob_order.txt",'w')
file.write(str(total_number) + "\n")
file.write(str(total_width) + "\n")
#输出序列
for i in range(1,total_number):
    file.write(str(i) + " " + str(width[i - 1]) + " " + str(height[i-1]) + "\n")
file.write(str(total_number) + " " + str(width[record]) + " " + str(height[record]))
file.close()

#part3
#对宽度进行从大到小排序
width.reverse()
height.reverse()

file = open(name+"_weight_btos_order.txt",'w')
file.write(str(total_number) + "\n")
file.write(str(total_width) + "\n")

#输出格式化序列
for i in range(1,total_number):
    file.write(str(i) + " " + str(width[i - 1]) + " " + str(height[i-1]) + "\n")
file.write(str(total_number) + " " + str(width[record]) + " " + str(height[record]))
file.close()

order_list = zip(height,width)
order_list = list(order_list)

#part4
#对高度进行从小到大排序
sort_result = sorted(order_list,key=lambda x:x[0])
i = 0
for x in sort_result:
    height[i],width[i] = x
    i = i + 1

file = open(name+"_height_stob_order.txt",'w')
file.write(str(total_number) + "\n")
file.write(str(total_width) + "\n")
#输出格式化序列
for i in range(1,total_number):
    file.write(str(i) + " " + str(width[i - 1]) + " " + str(height[i-1]) + "\n")
file.write(str(total_number) + " " + str(width[record]) + " " + str(height[record]))
file.close()

#part5
#对高度进行从大到小排序
width.reverse()
height.reverse()

file = open(name+"_height_btos_order.txt",'w')
file.write(str(total_number) + "\n")
file.write(str(total_width) + "\n")

#输出格式化序列
for i in range(1,total_number):
    file.write(str(i) + " " + str(width[i - 1]) + " " + str(height[i-1]) + "\n")
file.write(str(total_number) + " " + str(width[record]) + " " + str(height[record]))
file.close()