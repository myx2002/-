import random
import subprocess
program1 = "C:/Users/ma/Desktop/homework2.exe"
program2 = "C:/Users/ma/Desktop/test2.exe"


def duplicate(upper_bound):
    param = f"{upper_bound}\n"
    num = list(range(upper_bound+1))
    num.remove(0)
    random.shuffle(num)
    for i in num:
        param += str(i)
        param += " "
    return param
    

def construct_param():
    param = duplicate(18)
    return param

times = 0
while( True ):
    param = construct_param().encode("utf-8")
    output1 = subprocess.Popen([program1], stdin = subprocess.PIPE,stdout=subprocess.PIPE).communicate(param)[0].decode().strip()
    output2 = subprocess.Popen([program2], stdin = subprocess.PIPE,stdout=subprocess.PIPE).communicate(param)[0].decode().strip()
    if(output1==output2):
        times = times  + 1
        print(f"pass {times}")
    else:
        print(param.decode())
        print("program1:")
        print(output1)
        print("program2:")
        print(output2)
        break