#include"myclass.h"
#include<string>
#include<iostream>
using namespace std;

//改变日期内容函数（直接赋值）
void diary::set(string da, string en)
{
	date = da;
	entity = en;
}
//输出日期内容至命令行界面
void diary::print()
{
	cout << date << ":" << endl;
	cout << entity << endl;
}
//交换指定对象的日期与内容值
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
//判断对象日期是否与给定日期一致
int diary::pdcheck(string da)
{
	if (da == date)
		return 1;
	else
		return 0;
}
//返回日期变变量
string diary::viewdate()
{
	return date;
}
//返回日记变量
string diary::viewentity()
{
	return entity;
}