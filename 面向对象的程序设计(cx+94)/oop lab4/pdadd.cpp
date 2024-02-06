#include<iostream>
#include<string>
#include<vector>
#include"myclass.h"
using namespace std;
//增加日记函数
void pdadd(vector<diary>& newarray)
{
	string entity = "";
	string date;
	string temp;
	diary* temparray = new diary;
	int result, i;
	//提示输入日记日期与内容
	cout << "Please enter the insert date:" << endl;
	cin >> date;
	cout << "Please enter the insert entity:" << endl;
	
	//以.为结束，对输入内容进行跨行读取
	getline(cin, temp);
	while (1)
	{
		entity = entity + temp;
		if (temp.find(".") != -1)
		{
			(*temparray).set(date, entity);
			break;
		}
		getline(cin, temp);
	}

	//遍历已有日记日期
	for (i = 0;i < newarray.size();i++)
	{
		result = newarray[i].pdcheck((*temparray).viewdate());
		if (result == 1)
		{
			//若输入日期已存在，则直接更新当日日记
			newarray[i].set(newarray[i].viewdate(), entity);
			cout << "Modification occurred on " << date << " ." << endl;
			break;
		}
	}
	//若输入日期不存在，则在日记册末尾增添新的日记
	if (i == newarray.size())
	{
		newarray.push_back(*temparray);
		cout << "Insert successful!" << endl;
	}
	delete temparray;
}