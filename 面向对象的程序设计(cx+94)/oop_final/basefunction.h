#pragma once
//---**通用函数**---
//二进制减法
void sub_int(std::vector<int>& int_array, int index);
//二进制取反
void reverse(std::vector<int>& int_array);
//输出至文件
void print_to_file(std::vector<int>& int_array, std::ofstream& fout);
//从文件读入
void read_from_file(std::vector<int>& int_array, int total, std::ifstream& fin);
//二进制数转变为整数
int binary_to_int(std::vector<int>& int_array, int total);
//对于长整数
long long int binary_to_llint(std::vector<int>& int_array, int total);
//整数转变为二进制数
//模板直接定义于.h文件
template<typename T>
void int_to_binary(std::vector<int>& int_array, T number, int total)
{
	int index;
	//根据位操作，逐一取整数位放入整数数组中
	for (index = total - 1;index >= 0;index--)
		int_array.push_back((number >> index) & 1);
}