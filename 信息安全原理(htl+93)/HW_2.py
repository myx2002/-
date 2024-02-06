#交换a、b数值
def swap(a,b):
    return b,a

#比较a、b大小，若a>b则返回1，不然返回0
def check(a,b):
    flag = 0
    #不等长时比较
    if len(a)>len(b):
        flag=1
    #等长时比较
    elif len(a)==len(b):
        if a>=b:
            flag=1
    return  flag

#执行加法操作
def add(a,b):
    flag = check(a, b)
    if flag == 0:
        a, b = swap(a, b)
    c=0
    result=''
    #从最后一位开始计算加法
    a=a[::-1]
    b=b[::-1]
    for i in range(0,len(a)):
        #当两数均有位时
        if i < len(b):
            ch1 = ord(a[i])-ord('0')
            ch2 = ord(b[i])-ord('0')
            sum = ch1 + ch2 + c
        #一数完全增加后
        else:
            sum = ord(a[i])-ord('0')+c
        #管理进位情况
        if sum >= 10:
            sum = sum - 10
            c = 1
        else:
            c = 0
        result = chr(sum+ord('0')) + result
    #解决溢出进位的情况
    if c==1:
        result='1'+result
    return result

#执行减法操作
def sub(a,b):
    flag = check(a, b)
    if flag == 0:
        a, b = swap(a, b)
    c = 0
    result = ''
    #从最后一位开始逐位相减
    a = a[::-1]
    b = b[::-1]
    for i in range(0, len(a)):
        #两数对映位逐位相减
        if i < len(b):
            ch1 = ord(a[i]) - ord('0')
            ch2 = ord(b[i]) - ord('0')
            sum = ch1 - ch2 + c
        #多余位直接赋值（注意借位管理）
        else:
            sum = ord(a[i]) - ord('0') + c
        #管理借位问题
        if sum < 0:
            sum = sum + 10
            c = -1
        else:
            c = 0
        result = chr(sum + ord('0')) + result
    #删去数据中前置的无效‘0’
    result=result.lstrip('0')
    #输出管理，无论结果如何，保证输出为字符非数字
    if flag==0 :
        result='-'+result
    if result=='-' or len(result)==0:
        result='0'
    return result

#执行乘法操作
def mul(a,b):
    flag = check(a, b)
    if flag == 0:
        a, b = swap(a, b)
    result = '0'
    b = b[::-1]
    #无需注意顺序，直接相乘即可（借用加法运算）
    for i in range(0, len(b)):
        j=0
        while j < (ord(b[i])-ord('0')):
            result=add(result,a)
            j=j+1
        #提速步骤，不断增大每步增加值，大大减少运行时间
        a=a+'0'
    return result

#执行除法操作
def div(a,b,com=0):
    result=a
    ch=0
    flag = check(a, b)
    #除数小于被除数时，商只能为被除数
    if flag==0 and com==0:
        return '0'
    elif flag==0 and com==1:
        return a
    while check(result,b)!=0:
        su=b
        c=1
        #效仿列竖式，从最大可减项开始计算，加速程序运行
        while check(result,su)>0:
            su=su+'0'
            c=c*10
        #返回最后一次多余操作
        c=c//10
        su=su[0:len(su)-1]
        result=sub(result,su)
        ch=ch+c
    #若该函数被取模函数调用，选择返回余数值
    if com!=0:
        return result
    result=''
    #将整型记录的数据转化为字符串
    while ch>0:
        result=chr(ch%10+ord('0'))+result
        ch=ch//10
    return result

#执行取模操作
def mo(a,b):
    #调用除数函数的第二种形式，直接返回余数值
    result=div(a,b,1)
    return result

#进行第一个模块
print("part 2-1:")
#a=input("the first big number(support negative numbers):")
#b=input("the second big number(cannot be zero):")
a='0'
b='-66'

#支持负数输入，考虑字符串携带负号时的情况，进行对映的变化运算
#统计符号个数及分布
min=0
if a[0]=='-':
    min = 1
    a = a.lstrip('-')
if b[0]=='-':
    if min==1:
        min=3
    else:
        min = 2
    b = b.lstrip('-')

#切换加减法形态
if min==0:
    result1=add(a,b)
    result2=sub(a, b)
elif min==1:
    result1 = sub(b, a)
    result2 = '-' + add(a, b)
elif min==2:
    result1 = sub(a, b)
    result2 = add(a, b)
elif min==3:
    result1='-'+add(a,b)
    result2 = sub(b,a)

#切换乘数法形态
if min==1 or min==2:
    result3 = mul(a, b)
    result4 = div(a, b)
    if result3!='0':
        result3='-'+result3
    if result4 != '0':
        result4 = '-' + result4
else:
    result3=mul(a,b)
    result4=div(a,b)

#切换趋于函数形态
if min==1 or min==3:
    result5='-'+mo(a,b)
else:
    result5 = mo(a, b)

#输出运算结果
print(result1)
print(result2)
print(result3)
print(result4)
print(result5)

#进行第二个模块
print("part 2-2:")
p=input("the number p:")
q=input("the number q:")
a=int(input('A\'s private key:'))
b=int(input('B\'s private key:'))

#初始化输出值
resulta='1'
resultb='1'
resultc='1'

#计算A的公钥
for i in range(0,a):
    resulta=mul(resulta,q)
resulta = mo(resulta, p)

#计算B的公钥与协定执行密钥
for i in range(0, b):
    resultb = mul(resultb, q)
    resultc = mul(resultc, resulta)
resultb=mo(resultb,p)
resultc=mo(resultc,p)

#输出计算结果
print(resulta)
print(resultb)
print(resultc)