//---**二进制序列化与反序列化通用函数**---
#include<iostream>
#include<fstream>
#include<vector>
using namespace std;
#include "basefunction.h"
//二进制减法
//按位减1，与取反相联系
void sub_int(vector<int>& int_array, int index)
{
	int carry = 1;
	//从0开始，总数减1
	index = index - 1;
	while (index >= 0)
	{
		int_array[index] -= carry;
		//根据是否借位更新carry与当前位数值
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
//二进制取反
//按位取反即可
void reverse(vector<int>& int_array)
{
	for (int index = 0;index <= 31;index++)
		int_array[index] = 1 - int_array[index];
}
//输出至文件
void print_to_file(vector<int>& int_array, ofstream& fout)
{
	//使用迭代器方便容器输入到文件
	for (auto iter = int_array.begin();iter != int_array.end();iter++)
		fout << *iter;
}
//从文件读入
void read_from_file(vector<int>& int_array, int total, ifstream& fin)
{
	char receive;
	int int_type;
	for (int index = 0;index <= total - 1;index++)
	{
		fin >> receive;
		//将字符转换为整数，同一以整数记录至对映vector数组中
		int_type = int(receive - '0');
		int_array.push_back(int_type);
	}
}
//二进制数转变为整数
int binary_to_int(vector<int>& int_array, int total)
{
	int result = 0;
	//通用办法转换
	//从后向前转换，逐渐叠加
	for (int index = total - 1;index >= 0;index--)
		result += pow(2, total - 1 - index) * int_array[index];
	return result;
}
//对于长整数
//主要应对于double类型的二进制整数要求（32位与64位）
long long int binary_to_llint(vector<int>& int_array, int total)
{
	long long int result = 0;
	for (int index = total - 1;index >= 0;index--)
		result += pow(2, total - 1 - index) * int_array[index];
	return result;
}