#include<iostream>
#include<string>
#include<vector>
#include"myclass.h"
using namespace std;

//��������ռ��������ݺ���
void pdlist(vector<diary>& newarray)
{
	//�������ռ��������ڴ�С��������
	for (int i = 0;i < newarray.size();i++)
		for (int j = i + 1;j < newarray.size();j++)
			if (newarray[i].viewdate() > newarray[j].viewdate())
				newarray[i].swap(&newarray[j]);
	int i, flag = 0;
	string start;
	string end;

	//��ʾ������Ҫ��ѯ�ռǵ����ڷ�Χ
	//ÿ�����ڰ�λ���·�������0���λ
	cout << "Please enter the start date:��input 0 to show all)" << endl;
	cout << "There are a total of eight seats on YYYYMMDD and fill in empty spaces with 0." << endl;
	cin >> start;
	//����Ϊ0�����ʾȫ����ѯ
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
	//��������ʱ����ֻ�����Χ���ռ�����
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
	//����ѯ���ڷ�Χ�ڲ������ռǣ������none
	if (flag == 0)
		cout << "None!" << endl;
}