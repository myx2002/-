#include<iostream>
#include<string>
#include<vector>
#include"myclass.h"
using namespace std;
//����ɾ������
void pdremove(vector<diary>& newarray)
{
	string date;
	int result, i;
	int size = newarray.size();
	//��ʾ������Ҫɾ����ָ������
	cout << "Please enter the delete date:" << endl;
	cin >> date;
	for (i = 0;i < size;i++)
	{
		result = newarray[i].pdcheck(date);
		if (result == 1)
		{
			//��ָ�����ڴ��ڣ���ʹ�������һ���ռǽ�������ɾ�����һ���ռ�
			newarray[i].swap(&newarray[size - 1]);
			newarray.pop_back();
			break;
		}
	}
	//��ɾ���ɹ����򷵻�0��ɾ��ʧ�ܣ��򷵻�-1
	if (i == size)
		cout << "-1" << endl;
	else
		cout << "0" << endl;
}