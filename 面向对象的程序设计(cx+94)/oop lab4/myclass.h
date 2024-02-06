#pragma once
#include<iostream>
#include<string>
#include<vector>
using namespace std;

//定义日记类型
class diary {
private:
	//日期变量
	string date;
	//实体变量
	string entity;

public:
	//改变对象日期内容函数
	void set(string da, string en);
	//输出函数
	void print();
	//交换对象内容函数
	void swap(diary* num);
	//获得日期变量函数
	string viewdate();
	//获得内容函数
	string viewentity();
	//比对日期函数
	int pdcheck(string da);
};

//删除处理函数
void pdremove(vector<diary>& newarray);
//指定日期展示函数
void pdshow(vector<diary>& newarray);
//列表展示函数
void pdlist(vector<diary>& newarray);
//添加日历函数
void pdadd(vector<diary>& newarray);

