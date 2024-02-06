#pragma once
//---**普通类型二进制转变部分**---
// 统一输出格式说明
// 1.输入容器类型
// 2.文件名
// 3.相对于文件头偏移量
namespace to_binary
{
	//int转变
	//序列化函数
	int serialize_bi(int int_type, std::string name, int offset);
	//反序列化函数
	int deserialize_bi(int& int_rece, std::string name, int offset);

	//float转变
	//序列化函数
	int serialize_bi(float float_type, std::string name, int offset);
	//反序列化函数
	int deserialize_bi(float& float_rece, std::string name, int offset);

	//char转变
	//序列化函数
	int serialize_bi(char char_type, std::string name, int offset);
	//反序列化函数
	int deserialize_bi(char& char_rece, std::string name, int offset);

	//bool类型转变
	//序列化函数
	int serialize_bi(bool bool_type, std::string name, int offset);
	//反序列化函数
	int deserialize_bi(bool& bool_rece, std::string name, int offset);

	//double类型转变
	//序列化函数
	int serialize_bi(double double_type, std::string name, int offset);
	//反序列化函数
	int deserialize_bi(double& double_rece, std::string name, int offset);

	//string类型转变
	//序列化函数
	int serialize_bi(std::string string_type, std::string name, int offset);
	//反序列化函数
	int deserialize_bi(std::string& string_rece, std::string name, int offset);
}
extern std::ifstream fin;
extern std::ofstream fout;
	