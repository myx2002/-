#include<iostream>
#include<string>
#include<vector>
#include"myclass.h"
using namespace std;
//�����ռǺ���
void pdadd(vector<diary>& newarray)
{
	string entity = "";
	string date;
	string temp;
	diary* temparray = new diary;
	int result, i;
	//��ʾ�����ռ�����������
	cout << "Please enter the insert date:" << endl;
	cin >> date;
	cout << "Please enter the insert entity:" << endl;
	
	//��.Ϊ���������������ݽ��п��ж�ȡ
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

	//���������ռ�����
	for (i = 0;i < newarray.size();i++)
	{
		result = newarray[i].pdcheck((*temparray).viewdate());
		if (result == 1)
		{
			//�����������Ѵ��ڣ���ֱ�Ӹ��µ����ռ�
			newarray[i].set(newarray[i].viewdate(), entity);
			cout << "Modification occurred on " << date << " ." << endl;
			break;
		}
	}
	//���������ڲ����ڣ������ռǲ�ĩβ�����µ��ռ�
	if (i == newarray.size())
	{
		newarray.push_back(*temparray);
		cout << "Insert successful!" << endl;
	}
	delete temparray;
}