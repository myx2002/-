#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include"basefunction.h"
#include"type_binary.h"
using namespace std;
//---**��ͨ���Ͷ�����ת�䲿��**---
//intת��
//���л�����
int to_binary::serialize_bi(int int_type, std::string name, int offset)
{
	fout.open(name, ios::app);
	vector<int> int_array;
	//������������ֱ�ӵ���ͨ�ú���ת��Ϊ�����Ƽ���
	int_to_binary(int_array, int_type, 32);
	//ʹ�ö�����������ֵд���ļ�
	print_to_file(int_array, fout);
	fout.close();
	//ƫ����Ϊ�ֽ���*8
	return sizeof(int) * 8;
}
//�����л�����
int to_binary::deserialize_bi(int& int_rece, std::string name, int offset)
{
	vector<int>int_array;
	int flag = 0;
	fin.open(name, ios::in);
	//������Ҫ���ݸ�����ƫ����ƽ���ļ�ָ�룬ʹ��ָ��������Ҫ������
	fin.seekg(offset, ios::beg);
	//�������ݵ�����
	read_from_file(int_array, 32, fin);
	if (int_array[0] == 1)
	{
		flag = 1;
		//������Ϊ��������Ҫ���м�1ȡ���ķ������
		sub_int(int_array, 32);
		reverse(int_array);
	}
	//��ͨ�ú�����������ת��Ϊ����
	int_rece = binary_to_int(int_array, 32);
	if (flag == 1)
		int_rece = -int_rece;
	//cout << int_rece;
	fin.close();
	return sizeof(int) * 8;
}

//floatת��
//���л�����
int to_binary::serialize_bi(float float_type, std::string name, int offset)
{
	fout.open(name, ios::app);
	vector<int>int_array;
	//�ؼ����裬ֱ�ӽ�float��Ӧ�Ķ���������Ϊ���ͣ�����Զ���������Ͳ��������޸�ԭ��������
	int int_type = *(int*)(&float_type);
	//������ת��
	int_to_binary(int_array, int_type, 32);
	//д���ӳ�ļ�
	print_to_file(int_array, fout);
	fout.close();
	return sizeof(float) * 8;
}
//�����л�����
int to_binary::deserialize_bi(float& float_rece, std::string name, int offset)
{
	fin.open(name, ios::in);
	//��Ҫƽ���ļ�ָ����ٶ�ȡ
	fin.seekg(offset, ios::beg);
	int result;
	int flag = 0;
	vector<int>int_array;
	//�����������ж�������
	read_from_file(int_array, 32, fin);
	if (int_array[0] == 1)
	{
		//��1ȡ���ȶ����ƶ�ӳ����
		sub_int(int_array, 32);
		reverse(int_array);
		flag = 1;
	}
	//��������ת��Ϊ����
	result = binary_to_int(int_array, 32);
	if (flag == 1)
		result = -result;
	//ͨ��ָ���������������ͬ���Ķ���������ת��Ϊfloat����
	float_rece = *(float*)(&result);
	fin.close();
	return sizeof(float) * 8;
}

//charת��
//���л�����
int to_binary::serialize_bi(char char_type, std::string name, int offset)
{
	fout.open(name, ios::app);
	int transfor;
	//char���Ϳ���ֱ��ת��Ϊ����
	transfor = int(char_type);
	vector<int> int_array;
	//���ж�Ӧ�Ķ�����ת����д���ļ���������
	int_to_binary(int_array, transfor, 8);
	print_to_file(int_array, fout);
	fout.close();
	return sizeof(char) * 8;
}
//�����л�����
int to_binary::deserialize_bi(char& char_rece, std::string name, int offset)
{
	fin.open(name, ios::in);
	//ƫ��ָ������ӳ��ƫ������
	fin.seekg(offset, ios::beg);
	int result;
	vector<int>int_array;
	//���ļ��ж�����ӳ����
	read_from_file(int_array, 8, fin);
	//ת��Ϊ����
	result = binary_to_int(int_array, 8);
	//ֱ�ӽ����͸���char���ͼ��ɣ�һ���������Լ���ضϣ�һ�������ͱ�����char����ת�����������Խ��
	char_rece = result;
	fin.close();
	return sizeof(char) * 8;
}

//bool����ת��
//���л�����
int to_binary::serialize_bi(bool bool_type, std::string name, int offset)
{
	fout.open(name, ios::app);
	//bool����ֻ���ж��Ƿ�Ϊtrue�������--����Ϊ����
	if (bool_type)
		fout << 1;
	else
		fout << 0;
	fout.close();
	return 1;
}
//�����л�����
int to_binary::deserialize_bi(bool& bool_rece, std::string name, int offset)
{
	fin.open(name, ios::in);
	//һ��Ҫ����ƫ��
	fin.seekg(offset, ios::beg);
	//����char���ͣ�ֱ��ת��Ϊ���ͼ���
	char receive;
	fin >> receive;
	int result = int(receive - '0');
	//����ֻ��1/0ת��Ϊ���ͣ����ת����bool����ʱ�����������Խ������
	bool_rece = result;
	//cout << bool_rece;
	fin.close();
	return 1;
}

//double����ת��
//���л�����
int to_binary::serialize_bi(double double_type, std::string name, int offset)
{
	fout.open(name, ios::app);
	vector<int>int_array;
	//������float��������ͨ��ָ�뽫double���͵Ķ����Ƹ�������
	//�ر�ע�⣬��Ҫʹ��longlongint���ͣ���ֹ64λ->32λת��ʱ��������ض�
	long long int int_type = *(long long int*)(&double_type);
	//ת��Ϊ�����ַ������Ʋ�д���ļ�
	int_to_binary(int_array, int_type, 64);
	print_to_file(int_array, fout);
	fout.close();
	return sizeof(float) * 16;
}
//�����л�����
int to_binary::deserialize_bi(double& double_rece, std::string name, int offset)
{
	fin.open(name, ios::in);
	//ƽ���ļ�ָ��ָ��
	fin.seekg(offset, ios::beg);
	long long int result;
	int flag = 0;
	vector<int>int_array;
	//��������
	read_from_file(int_array, 64, fin);
	//�ж�double�����������
	if (int_array[0] == 1)
	{
		//���ж�Ӧ�ļ�1ȡ������
		sub_int(int_array, 64);
		reverse(int_array);
		flag = 1;
	}
	//������ת��Ϊlonglongint����
	result = binary_to_llint(int_array, 64);
	if (flag == 1)
		result = -result;
	//��������ָ�룬����Ӧ�����Ͷ�����ת��Ϊdouble����
	double_rece = *(double*)(&result);
	fin.close();
	return sizeof(float) * 16;
}

//string����ת��
//���л�����
int to_binary::serialize_bi(std::string string_type, std::string name, int offset)
{
	fout.open(name, ios::app);
	vector<int> int_array;
	//���ȼ�¼string���͵Ĵ�С
	int total_string = string_type.size();
	//�ȼ�¼��С��ӳ������
	int_to_binary(int_array, total_string, 32);
	print_to_file(int_array, fout);
	int index;
	int int_string;
	//����char���͵ĸ�����һһ���ļ�д��char��������
	for (index = 0;index < total_string;index++)
	{
		//ÿ��clear�������飬��ֹ���ݼ��໥Ӱ��
		int_array.clear();
		int_string = int(string_type[index]);
		//����ӳ��char����ת��Ϊ�����ƺ�д���ļ�
		int_to_binary(int_array, int_string, 8);
		print_to_file(int_array, fout);
	}
	fout.close();
	return sizeof(int) * 8 + sizeof(char) * total_string * 8;
}
//�����л�����
int to_binary::deserialize_bi(std::string& string_rece, std::string name, int offset)
{
	string_rece = "";
	fin.open(name, ios::in);
	//ƫ���ļ�ָ������ӳ�������ݵĵط�
	fin.seekg(offset, ios::beg);
	int total_string;
	vector<int>int_array;
	//�Ȼ��string���ݵĴ�С������һһȡ��char��������
	read_from_file(int_array, 32, fin);
	//����������ת��Ϊ����
	total_string = binary_to_int(int_array, 32);
	int index;
	int result;
	//һһȡ��char��������
	for (index = 0;index < total_string;index++)
	{
		int_array.clear();
		//�������ݲ���������ת��Ϊ����
		read_from_file(int_array, 8, fin);
		result = binary_to_int(int_array, 8);
		//string���Ϳ���ֱ�����ӻ�ý��
		string_rece += char(result);
	}
	fin.close();
	return sizeof(int) * 8 + sizeof(char) * total_string * 8;
}
