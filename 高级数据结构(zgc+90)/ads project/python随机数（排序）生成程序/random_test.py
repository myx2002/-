import random
import time
random.seed(time.time())
#读入总个数，最大宽度
#读入大矩形占比因子，输出文件名
#total_number = int(input("input total number："))
total_number = 10000
#total_width = int(input("input total width："))
total_width = 120
rate = float(input("input the rate: "))
name = input("input the txt_name: ")
#record = total_number - 1;
width = []
height = []

#通过要求生成的个数计算小矩形与大矩形的生成个数
number = int((total_number + 1) * rate)
normal = total_number + 1 - number
#随机生成大小宽度序列
for i in range(1,normal + 1):
    width.append(random.randint(1,11))
for i in range(1,number):
    width.append(random.randint(1,total_width))
random.shuffle(width)

for i in range(1,total_number+1):
    height.append(random.randint(1,11))
random.shuffle(height)
#随机生成大小高度序列

file = open(name+"_height.txt",'w')
file.write(str(total_number) + "\n")
file.write(str(total_width) + "\n")
#输出格式化序列
for i in range(1,total_number + 1 ):
    file.write(str(i) + " " + str(width[i - 1]) + " " + str(height[i - 1]) + "\n")
file.close()


'''
for i in range(1,normal + 1):
    height.append(random.randint(1,11))
for i in range(1,number):
    height.append(random.randint(1,total_width))
random.shuffle(height)
#print(height)

#part1
#按随机顺序输出正方形
file = open(name+"_random.txt",'w')
file.write(str(total_number) + "\n")
file.write(str(total_width) + "\n")
#仅生成正方形
#指定输出格式
for i in range(1,total_number):
    file.write(str(i) + " " + str(width[i - 1]) + " " + str(width[i-1]) + "\n")
file.write(str(total_number) + " " + str(width[record]) + " " + str(width[record]))
file.close()

#part2
#按从小到大顺序输出正方形
width.sort()
file = open(name+"_stob_order.txt",'w')
file.write(str(total_number) + "\n")
file.write(str(total_width) + "\n")
#仅生成正方形
#指定输出格式
for i in range(1,total_number):
    file.write(str(i) + " " + str(width[i - 1]) + " " + str(width[i-1]) + "\n")
file.write(str(total_number) + " " + str(width[record]) + " " + str(width[record]))
file.close()

#part3
#按从大到小顺序输出正方形
width.sort(reverse = True)
file = open(name+"_btos_order.txt",'w')
file.write(str(total_number) + "\n")
file.write(str(total_width) + "\n")
#仅生成正方形
#指定输出格式
for i in range(1,total_number):
    file.write(str(i) + " " + str(width[i - 1]) + " " + str(width[i-1]) + "\n")
file.write(str(total_number) + " " + str(width[record]) + " " + str(width[record]))
file.close()
#print(width)
'''