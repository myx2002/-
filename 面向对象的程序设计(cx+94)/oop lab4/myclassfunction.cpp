#include"myclass.h"
#include<string>
#include<iostream>
using namespace std;

//�ı��������ݺ�����ֱ�Ӹ�ֵ��
void diary::set(string da, string en)
{
	date = da;
	entity = en;
}
//������������������н���
void diary::print()
{
	cout << date << ":" << endl;
	cout << entity << endl;
}
//����ָ�����������������ֵ
void diary::swap(diary* num)
{
	string temp;
	temp = num->date;
	num->date = date;
	date = temp;
	temp = num->entity;
	num->entity = entity;
	entity = temp;
}
//�ж϶��������Ƿ����������һ��
int diary::pdcheck(string da)
{
	if (da == date)
		return 1;
	else
		return 0;
}
//�������ڱ����
string diary::viewdate()
{
	return date;
}
//�����ռǱ���
string diary::viewentity()
{
	return entity;
}