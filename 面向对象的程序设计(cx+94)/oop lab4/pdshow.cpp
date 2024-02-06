#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include"myclass.h"
using namespace std;
//指定日期日记显示函数
void pdshow(vector<diary>& newarray)
{
	string date;
	int result;
	//提示输入需要查询的日记日期
	cout << "Please enter the query date:" << endl;
	cin >> date;
	int i;
	for (i = 0;i < newarray.size();i++)
	{
		result = newarray[i].pdcheck(date);
		//日期日记存在，则输出该日记
		if (result == 1)
		{
			newarray[i].print();
			break;
		}
	}
	//若日期日记不存在，则返回不存在提示
	if (i == newarray.size())
		cout << date << " does not exist!" << endl;
}