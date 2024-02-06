#include<iostream>
#include<string>
#include<vector>
#include"myclass.h"
using namespace std;

//批量输出日记日期内容函数
void pdlist(vector<diary>& newarray)
{
	//对已有日记依赖日期大小进行排序
	for (int i = 0;i < newarray.size();i++)
		for (int j = i + 1;j < newarray.size();j++)
			if (newarray[i].viewdate() > newarray[j].viewdate())
				newarray[i].swap(&newarray[j]);
	int i, flag = 0;
	string start;
	string end;

	//提示输入需要查询日记的日期范围
	//每个日期八位，月份日期用0填补空位
	cout << "Please enter the start date:（input 0 to show all)" << endl;
	cout << "There are a total of eight seats on YYYYMMDD and fill in empty spaces with 0." << endl;
	cin >> start;
	//输入为0，则表示全部查询
	if (start == "0")
	{
		for (int i = 0;i < newarray.size();i++)
			newarray[i].print();
		cout << endl;
		return;
	}
	cout << "Please enter the end date:" << endl;
	cout << "There are a total of eight seats on YYYYMMDD and fill in empty spaces with 0." << endl;
	cin >> end;
	//其他输入时，则只输出范围内日记内容
	for (i = 0;i < newarray.size();i++)
	{
		if ((newarray[i].viewdate() >= start && newarray[i].viewdate() <= end) ||
			newarray[i].viewdate().length() > start.length() && newarray[i].viewdate().length() < end.length())
		{
			newarray[i].print();
			cout << endl;
			flag = 1;
		}
	}
	//若查询日期范围内不存在日记，则输出none
	if (flag == 0)
		cout << "None!" << endl;
}