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

	//�����ļ������
	ifstream fcin;
	//���ļ�
	fcin.open("try.txt",ios::in);
	//����ʧ�ܣ��򷵻�ʧ����ʾ
	if (!fcin.is_open())
	{
		cout << "File open failed!" << endl;
		fcin.close();
		return 0;
	}
	//ѭ����������������ռ�����
	while (1)
	{
		fcin >> date;
		//���Ѷ����ļ�ĩβ�����˳�
		if (fcin.eof())
			break;
		//�Զ��������з�
		getline(fcin,temp);
		entity = "";
		while (1)
		{
			//����ƴ�Ͽ��е��ռ�����
			entity = entity + temp;
			//����.��Ϊÿһ�������ռ����ݵĽ���
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
	//�ر��ļ�������
	fcin.close();

	int receive;
	//��ʼʵ���ռǹ�����
	while (1)
	{
		//��ʾ������������������������Ч
		cout << "Welcome to personal diary!" << endl;
		cout << "1.pdadd" << endl;
		cout << "2.pdlist" << endl;
		cout << "3.pdshow" << endl;
		cout << "4.pdremove" << endl;
		cout << "5.quit" << endl;
		cout << "Please enter your action(in digit):" << endl;
		cin >> receive;
		//���ݲ�ͬ���������룬ִ�в�ͬ�Ĳ�������
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
			//���ڲ��Ϸ����룬���������ʾ
			cout << "Incorrect input!" << endl;
		cout << endl;
	}

	//���ļ������
	ofstream fout;
	//���ļ�
	fout.open("try.txt", ios::out);
	//����ʧ�ܣ��򷵻ش�����Ϣ
	if (!fout.is_open())
	{
		cout << "File write failure!" << endl;
		fout.close();
		return 0;
	}
	//��������ռ�����������
	for (int i = 0;i < newarray.size();i++)
	{
		fout<<newarray[i].viewdate()<< endl;
		fout << newarray[i].viewentity() << endl;
	}
	//�ر��ļ����liu
	fout.close();
	return 0;
}