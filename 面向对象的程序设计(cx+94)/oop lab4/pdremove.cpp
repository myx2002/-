#include<iostream>
#include<string>
#include<vector>
#include"myclass.h"
using namespace std;
//日期删除函数
void pdremove(vector<diary>& newarray)
{
	string date;
	int result, i;
	int size = newarray.size();
	//提示输入需要删除的指定日期
	cout << "Please enter the delete date:" << endl;
	cin >> date;
	for (i = 0;i < size;i++)
	{
		result = newarray[i].pdcheck(date);
		if (result == 1)
		{
			//若指定日期存在，则使其与最后一项日记交换，再删除最后一项日记
			newarray[i].swap(&newarray[size - 1]);
			newarray.pop_back();
			break;
		}
	}
	//若删除成功，则返回0；删除失败，则返回-1
	if (i == size)
		cout << "-1" << endl;
	else
		cout << "0" << endl;
}