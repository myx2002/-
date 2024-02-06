#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include"myclass.h"
using namespace std;

int main()
{
	string date;
	string entity="";
	string temp;
	vector<diary>newarray;

	//定义文件输出流
	ifstream fcin;
	//打开文件
	fcin.open("try.txt",ios::in);
	//若打开失败，则返回失败提示
	if (!fcin.is_open())
	{
		cout << "File open failed!" << endl;
		fcin.close();
		return 0;
	}
	//循环读入日期与跨行日记内容
	while (1)
	{
		fcin >> date;
		//若已读至文件末尾，则退出
		if (fcin.eof())
			break;
		//自动丢弃换行符
		getline(fcin,temp);
		entity = "";
		while (1)
		{
			//不断拼合跨行的日记内容
			entity = entity + temp;
			//仅以.作为每一条跨行日记内容的结束
			if (temp.find(".") != -1)
			{
				diary* temparray = new diary;
				(*temparray).set(date, entity);
				newarray.push_back(*temparray);
				delete temparray;
				break;
			}
			getline(fcin, temp);
		}
	}
	//关闭文件读入流
	fcin.close();

	int receive;
	//开始实现日记管理功能
	while (1)
	{
		//提示输出各项操作，仅输入数字有效
		cout << "Welcome to personal diary!" << endl;
		cout << "1.pdadd" << endl;
		cout << "2.pdlist" << endl;
		cout << "3.pdshow" << endl;
		cout << "4.pdremove" << endl;
		cout << "5.quit" << endl;
		cout << "Please enter your action(in digit):" << endl;
		cin >> receive;
		//根据不同的数字输入，执行不同的操作函数
		if (receive == 5)
			break;
		else if (receive == 1)
			pdadd(newarray);
		else if (receive == 2)
			pdlist(newarray);
		else if (receive == 3)
			pdshow(newarray);
		else if (receive == 4)
			pdremove(newarray);
		else
			//对于不合法输入，输出错误提示
			cout << "Incorrect input!" << endl;
		cout << endl;
	}

	//打开文件输出流
	ofstream fout;
	//打开文件
	fout.open("try.txt", ios::out);
	//若打开失败，则返回错误信息
	if (!fout.is_open())
	{
		cout << "File write failure!" << endl;
		fout.close();
		return 0;
	}
	//遍历输出日记日期与内容
	for (int i = 0;i < newarray.size();i++)
	{
		fout<<newarray[i].viewdate()<< endl;
		fout << newarray[i].viewentity() << endl;
	}
	//关闭文件输出liu
	fout.close();
	return 0;
}