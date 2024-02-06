/*Author: 马宇啸  ID: 3210104530   NO.01 */
#include<stdio.h>
#include<stdlib.h>

//第一个栈用于贮存数据
typedef struct firsts {
    int number;
    struct firsts* next;
}one;

//第二个栈用于贮存操作符
typedef struct seconds {
    char operate;
    struct seconds* next;
}two;

int main()
{
    int input, index, flag = 1;

    //获取总共操作数数量
    scanf("%d", &input);

    //建立两个栈的头指针
    one* p1;
    one* p2;
    p1 = (one*)malloc(sizeof(one));
    p1->next = NULL;
    two* p3;
    two* p4;
    p3 = (two*)malloc(sizeof(two));
    p3->next = NULL;

    //获得全部操作数，存入数据栈中
    for (index = 0;index < input;index++)
    {
        p2 = (one*)malloc(sizeof(one));
        scanf("%d", &(p2->number));
        p2->next = p1;
        p1 = p2;
    }
    //消除换行符的干扰
    getchar();

    //获得全部操作符，存入操作符栈中
    for (index = 0;index < input - 1;index++)
    {
        p4 = (two*)malloc(sizeof(two));
        scanf("%c", &(p4->operate));
        getchar();
        p4->next = p3;
        p3 = p4;
    }

    //开始模拟计算
    int a, b, c;
    char ch;

    //不停循环，直至运算次数达到操作符数量
    while (input - 1 > 0)
    {
        //取出栈中数据与操作符
        a = p1->number;
        b = p1->next->number;
        ch = p3->operate;
        //栈向后偏移
        p1 = p1->next;
        p3 = p3->next;
        //根据操作符进行数据操作
        if (ch == '+')
            c = a + b;
        else if (ch == '-')
            c = b - a;
        else if (ch == '*')
            c = a * b;
        else
        {
            if (a == 0)
            {
                flag = 0;
                break;
            }
            else
                c = b / a;
        }
        //保留被除数
        p1->number = c;
        input--;
    }
    //如果存在除数为零的情况
    if (flag == 0)
        printf("ERROR: %d/0", b);
    else
        printf("%d", c);
    return 0;
}
