//---**���������л��뷴���л�ͨ�ú���**---
#include<iostream>
#include<fstream>
#include<vector>
using namespace std;
#include "basefunction.h"
//�����Ƽ���
//��λ��1����ȡ������ϵ
void sub_int(vector<int>& int_array, int index)
{
	int carry = 1;
	//��0��ʼ��������1
	index = index - 1;
	while (index >= 0)
	{
		int_array[index] -= carry;
		//�����Ƿ��λ����carry�뵱ǰλ��ֵ
		if (int_array[index] == -1)
		{
			int_array[index] = 1;
			carry = 1;
		}
		else
			carry = 0;
		index--;
	}
}
//������ȡ��
//��λȡ������
void reverse(vector<int>& int_array)
{
	for (int index = 0;index <= 31;index++)
		int_array[index] = 1 - int_array[index];
}
//������ļ�
void print_to_file(vector<int>& int_array, ofstream& fout)
{
	//ʹ�õ����������������뵽�ļ�
	for (auto iter = int_array.begin();iter != int_array.end();iter++)
		fout << *iter;
}
//���ļ�����
void read_from_file(vector<int>& int_array, int total, ifstream& fin)
{
	char receive;
	int int_type;
	for (int index = 0;index <= total - 1;index++)
	{
		fin >> receive;
		//���ַ�ת��Ϊ������ͬһ��������¼����ӳvector������
		int_type = int(receive - '0');
		int_array.push_back(int_type);
	}
}
//��������ת��Ϊ����
int binary_to_int(vector<int>& int_array, int total)
{
	int result = 0;
	//ͨ�ð취ת��
	//�Ӻ���ǰת�����𽥵���
	for (int index = total - 1;index >= 0;index--)
		result += pow(2, total - 1 - index) * int_array[index];
	return result;
}
//���ڳ�����
//��ҪӦ����double���͵Ķ���������Ҫ��32λ��64λ��
long long int binary_to_llint(vector<int>& int_array, int total)
{
	long long int result = 0;
	for (int index = total - 1;index >= 0;index--)
		result += pow(2, total - 1 - index) * int_array[index];
	return result;
}