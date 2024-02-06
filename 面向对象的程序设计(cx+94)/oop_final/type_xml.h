#pragma once
extern std::ifstream fin;
extern std::ofstream fout;

//---**一般数据类型转变为xml结构**---
// 统一输出格式说明（serialize_xml与deserialize_xml函数）
// 1.输入参数类型
// 2.文件标识名
// 3.文件名
// 4.是否为主写入调用（用户使用默认为1）

//get_xml函数
//1.对映数据类型结果存放参数
//2.文件输入流
//3.是否为主读入调用（用户使用默认为1）
namespace to_xml
{
	//int转换为xml
	//序列化主题函数，便于被容器自定义类调用
	void get_xml(int& result, std::ifstream& fin, int origin);
	//序列化函数
	void serialize_xml(int int_type, std::string name, std::string filename, int origin);
	//反序列化函数
	void deserialize_xml(int& int_type, std::string name, std::string filename, int origin);
	//bool转化为xml
	//序列化主题函数，便于被容器自定义类调用
	void get_xml(bool& result, std::ifstream& fin, int origin);
	//序列化函数
	void serialize_xml(bool bool_type, std::string name, std::string filename, int origin);
	//反序列化函数
	void deserialize_xml(bool& bool_type, std::string name, std::string filename, int origin);
	//char转化为xml
	//序列化主题函数，便于被容器自定义类调用
	void get_xml(char& result, std::ifstream& fin, int origin);
	//序列化函数
	void serialize_xml(char char_type, std::string name, std::string filename, int origin);
	//反序列化函数
	void deserialize_xml(char& char_type, std::string name, std::string filename, int origin);
	//float转化为xml
	//序列化主题函数，便于被容器自定义类调用
	void get_xml(float& result, std::ifstream& fin, int origin);
	//序列化函数
	void serialize_xml(float float_type, std::string name, std::string filename, int origin);
	//反序列化函数
	void deserialize_xml(float& float_type, std::string name, std::string filename, int origin);
	//double转化为xml
	//序列化主题函数，便于被容器自定义类调用
	void get_xml(double& result, std::ifstream& fin, int origin);
	//序列化函数
	void serialize_xml(double double_type, std::string name, std::string filename, int origin);
	//反序列化函数
	void deserialize_xml(double& double_type, std::string name, std::string filename, int origin);
	//string转化为xml
	//序列化主题函数，便于被容器自定义类调用
	void get_xml(std::string& result, std::ifstream& fin, int origin);
	//序列化函数
	void serialize_xml(std::string string_type, std::string name, std::string filename, int origin);
	//反序列化函数
	void deserialize_xml(std::string& string_type, std::string name, std::string filename, int origin);
}
