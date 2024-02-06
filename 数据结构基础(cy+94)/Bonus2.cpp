/*Author: ����Х  ID: 3210104530   NO.01 */
#include<stdio.h>
#include<stdlib.h>

//��һ��ջ������������
typedef struct firsts {
    int number;
    struct firsts* next;
}one;

//�ڶ���ջ�������������
typedef struct seconds {
    char operate;
    struct seconds* next;
}two;

int main()
{
    int input, index, flag = 1;

    //��ȡ�ܹ�����������
    scanf("%d", &input);

    //��������ջ��ͷָ��
    one* p1;
    one* p2;
    p1 = (one*)malloc(sizeof(one));
    p1->next = NULL;
    two* p3;
    two* p4;
    p3 = (two*)malloc(sizeof(two));
    p3->next = NULL;

    //���ȫ������������������ջ��
    for (index = 0;index < input;index++)
    {
        p2 = (one*)malloc(sizeof(one));
        scanf("%d", &(p2->number));
        p2->next = p1;
        p1 = p2;
    }
    //�������з��ĸ���
    getchar();

    //���ȫ�������������������ջ��
    for (index = 0;index < input - 1;index++)
    {
        p4 = (two*)malloc(sizeof(two));
        scanf("%c", &(p4->operate));
        getchar();
        p4->next = p3;
        p3 = p4;
    }

    //��ʼģ�����
    int a, b, c;
    char ch;

    //��ͣѭ����ֱ����������ﵽ����������
    while (input - 1 > 0)
    {
        //ȡ��ջ�������������
        a = p1->number;
        b = p1->next->number;
        ch = p3->operate;
        //ջ���ƫ��
        p1 = p1->next;
        p3 = p3->next;
        //���ݲ������������ݲ���
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
        //����������
        p1->number = c;
        input--;
    }
    //������ڳ���Ϊ������
    if (flag == 0)
        printf("ERROR: %d/0", b);
    else
        printf("%d", c);
    return 0;
}
