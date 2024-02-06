#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include"basefunction.h"
#include"type_binary.h"
using namespace std;
//---**普通类型二进制转变部分**---
//int转变
//序列化函数
int to_binary::serialize_bi(int int_type, std::string name, int offset)
{
	fout.open(name, ios::app);
	vector<int> int_array;
	//对于整数类型直接调用通用函数转化为二进制即可
	int_to_binary(int_array, int_type, 32);
	//使用二进制整型数值写入文件
	print_to_file(int_array, fout);
	fout.close();
	//偏移量为字节数*8
	return sizeof(int) * 8;
}
//反序列化函数
int to_binary::deserialize_bi(int& int_rece, std::string name, int offset)
{
	vector<int>int_array;
	int flag = 0;
	fin.open(name, ios::in);
	//首先需要根据给定的偏移量平移文件指针，使其指向我们需要的数据
	fin.seekg(offset, ios::beg);
	//读入数据到数组
	read_from_file(int_array, 32, fin);
	if (int_array[0] == 1)
	{
		flag = 1;
		//若数据为负数，需要进行减1取反的反向操作
		sub_int(int_array, 32);
		reverse(int_array);
	}
	//用通用函数将二进制转化为整型
	int_rece = binary_to_int(int_array, 32);
	if (flag == 1)
		int_rece = -int_rece;
	//cout << int_rece;
	fin.close();
	return sizeof(int) * 8;
}

//float转变
//序列化函数
int to_binary::serialize_bi(float float_type, std::string name, int offset)
{
	fout.open(name, ios::app);
	vector<int>int_array;
	//关键步骤，直接将float对应的二进制声明为整型，便可以对其进行整型操作而不修改原变量数据
	int int_type = *(int*)(&float_type);
	//二进制转化
	int_to_binary(int_array, int_type, 32);
	//写入对映文件
	print_to_file(int_array, fout);
	fout.close();
	return sizeof(float) * 8;
}
//反序列化函数
int to_binary::deserialize_bi(float& float_rece, std::string name, int offset)
{
	fin.open(name, ios::in);
	//需要平移文件指针后再读取
	fin.seekg(offset, ios::beg);
	int result;
	int flag = 0;
	vector<int>int_array;
	//从整型数组中读出数据
	read_from_file(int_array, 32, fin);
	if (int_array[0] == 1)
	{
		//减1取反等二进制对映操作
		sub_int(int_array, 32);
		reverse(int_array);
		flag = 1;
	}
	//将二进制转变为整型
	result = binary_to_int(int_array, 32);
	if (flag == 1)
		result = -result;
	//通过指针的类型声明，将同样的二进制数据转变为float类型
	float_rece = *(float*)(&result);
	fin.close();
	return sizeof(float) * 8;
}

//char转变
//序列化函数
int to_binary::serialize_bi(char char_type, std::string name, int offset)
{
	fout.open(name, ios::app);
	int transfor;
	//char类型可以直接转变为整型
	transfor = int(char_type);
	vector<int> int_array;
	//进行对应的二进制转换与写入文件操作即可
	int_to_binary(int_array, transfor, 8);
	print_to_file(int_array, fout);
	fout.close();
	return sizeof(char) * 8;
}
//反序列化函数
int to_binary::deserialize_bi(char& char_rece, std::string name, int offset)
{
	fin.open(name, ios::in);
	//偏移指针至对映的偏移量处
	fin.seekg(offset, ios::beg);
	int result;
	vector<int>int_array;
	//从文件中读出对映数据
	read_from_file(int_array, 8, fin);
	//转变为整型
	result = binary_to_int(int_array, 8);
	//直接将整型赋予char类型即可，一方面整型自己会截断，一方面整型本身由char类型转变而来，不会越界
	char_rece = result;
	fin.close();
	return sizeof(char) * 8;
}

//bool类型转变
//序列化函数
int to_binary::serialize_bi(bool bool_type, std::string name, int offset)
{
	fout.open(name, ios::app);
	//bool类型只需判定是否为true即可输出--本身即为整型
	if (bool_type)
		fout << 1;
	else
		fout << 0;
	fout.close();
	return 1;
}
//反序列化函数
int to_binary::deserialize_bi(bool& bool_rece, std::string name, int offset)
{
	fin.open(name, ios::in);
	//一定要做的偏移
	fin.seekg(offset, ios::beg);
	//读入char类型，直接转变为整型即可
	char receive;
	fin >> receive;
	int result = int(receive - '0');
	//本身只有1/0转换为整型，因而转换回bool类型时，并不会出现越界的情况
	bool_rece = result;
	//cout << bool_rece;
	fin.close();
	return 1;
}

//double类型转变
//序列化函数
int to_binary::serialize_bi(double double_type, std::string name, int offset)
{
	fout.open(name, ios::app);
	vector<int>int_array;
	//类似于float的做法，通过指针将double类型的二进制赋予整型
	//特别注意，需要使用longlongint类型，防止64位->32位转变时被不合理截断
	long long int int_type = *(long long int*)(&double_type);
	//转换为具体字符二进制并写入文件
	int_to_binary(int_array, int_type, 64);
	print_to_file(int_array, fout);
	fout.close();
	return sizeof(float) * 16;
}
//反序列化函数
int to_binary::deserialize_bi(double& double_rece, std::string name, int offset)
{
	fin.open(name, ios::in);
	//平移文件指向指针
	fin.seekg(offset, ios::beg);
	long long int result;
	int flag = 0;
	vector<int>int_array;
	//读出数组
	read_from_file(int_array, 64, fin);
	//判断double类型正负情况
	if (int_array[0] == 1)
	{
		//进行对应的减1取反操作
		sub_int(int_array, 64);
		reverse(int_array);
		flag = 1;
	}
	//二进制转变为longlongint类型
	result = binary_to_llint(int_array, 64);
	if (flag == 1)
		result = -result;
	//再依赖于指针，将对应的整型二进制转变为double类型
	double_rece = *(double*)(&result);
	fin.close();
	return sizeof(float) * 16;
}

//string类型转变
//序列化函数
int to_binary::serialize_bi(std::string string_type, std::string name, int offset)
{
	fout.open(name, ios::app);
	vector<int> int_array;
	//首先记录string类型的大小
	int total_string = string_type.size();
	//先记录大小对映的整型
	int_to_binary(int_array, total_string, 32);
	print_to_file(int_array, fout);
	int index;
	int int_string;
	//根据char类型的个数，一一向文件写入char类型数据
	for (index = 0;index < total_string;index++)
	{
		//每次clear整型数组，防止数据间相互影响
		int_array.clear();
		int_string = int(string_type[index]);
		//将对映的char类型转变为二进制后写入文件
		int_to_binary(int_array, int_string, 8);
		print_to_file(int_array, fout);
	}
	fout.close();
	return sizeof(int) * 8 + sizeof(char) * total_string * 8;
}
//反序列化函数
int to_binary::deserialize_bi(std::string& string_rece, std::string name, int offset)
{
	string_rece = "";
	fin.open(name, ios::in);
	//偏移文件指针至对映类型数据的地方
	fin.seekg(offset, ios::beg);
	int total_string;
	vector<int>int_array;
	//先获得string数据的大小，方便一一取出char类型数据
	read_from_file(int_array, 32, fin);
	//二进制整型转变为整型
	total_string = binary_to_int(int_array, 32);
	int index;
	int result;
	//一一取出char类型数据
	for (index = 0;index < total_string;index++)
	{
		int_array.clear();
		//读出数据并将二进制转变为整型
		read_from_file(int_array, 8, fin);
		result = binary_to_int(int_array, 8);
		//string类型可以直接作加获得结果
		string_rece += char(result);
	}
	fin.close();
	return sizeof(int) * 8 + sizeof(char) * total_string * 8;
}
