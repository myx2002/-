import random
x=[]
#产生基本插入数
for i in range(1, 51):
    x.append(i)
#按设定的比例生成 deletion 数
for i in range(1,51):
    s = random.choice((-1, 1, 2))
    if (s > 0):
        continue;
    else:
        x.append(i*-1)
#对上述list进行重排以增加 insert 与 deletion 的随机性
random.shuffle(x)
#写入文件
with open("C:/Users/ma/Desktop/data.txt",'w') as f:
    #统计总个数，方便后续测试运行
    f.write(str(len(x)))
    f.write('\n')
    for i in x:
        f.write(str(i))
        f.write(' ')


