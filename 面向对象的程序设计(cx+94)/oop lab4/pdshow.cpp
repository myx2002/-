#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include"myclass.h"
using namespace std;
//ָ�������ռ���ʾ����
void pdshow(vector<diary>& newarray)
{
	string date;
	int result;
	//��ʾ������Ҫ��ѯ���ռ�����
	cout << "Please enter the query date:" << endl;
	cin >> date;
	int i;
	for (i = 0;i < newarray.size();i++)
	{
		result = newarray[i].pdcheck(date);
		//�����ռǴ��ڣ���������ռ�
		if (result == 1)
		{
			newarray[i].print();
			break;
		}
	}
	//�������ռǲ����ڣ��򷵻ز�������ʾ
	if (i == newarray.size())
		cout << date << " does not exist!" << endl;
}