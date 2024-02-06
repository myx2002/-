#include<iostream>
#include<fstream>
#include<string>
#include"user_define_struct.h"
#include"type_xml.h"
#include"STL_xml.h"
#include"STL_binary.h"
#include"type_xml.h"
#include"basefunction_xml.h"
using namespace std;
extern std::ifstream fin;
extern std::ofstream fout;
//实现自定义模块的serialize与deserialize函数
//二进制序列化
int UserDefinedType::serialize_bi(string filename)
{
	//需要定义一偏离量
	int offset = 0;
	//只需要对类具有的元素调用serialize_xxx函数即可
	//特别注意，对于一般类型，使用to_binary空间的函数（“serialize_bi”函数）
	//			对于容器类型，需要使用to_binary_STL空间的函数（“serialize_容器名”函数）
	//每次函数调用函数后，需要更新偏移量
	offset += to_binary::serialize_bi(idx, filename, 0);
	offset += to_binary::serialize_bi(name, filename, 0);
	offset += to_binary_STL::serialize_vector(data, filename, 0);
	//最后返回偏移量即可
	return offset;
}
//二进制反序列化
int UserDefinedType::deserialize_bi(string filename)
{
	//需要定义一偏离量
	int offset = 0;
	//同理于序列化函数，只需调用对映namespace的deserialize_xxxx即可(规则同序列化函数）
	offset += to_binary::deserialize_bi(idx, filename, offset);
	offset += to_binary::deserialize_bi(name, filename, offset);
	offset += to_binary_STL::deserialize_vector(data, filename, offset);
	//不断更新偏移量并返回即可
	return offset;
}
//xml文件序列化
//区别于二进制转化，xml文件格式需要记录类对应的属性名称，因而需要更加的特殊化设计
void UserDefinedType::serialize_myxml(std::string name, std::string filename, std::vector<int>& total_number)
{
	//记录容器属性大小，与容器的序列反序列统一
	total_number.push_back(data.size());
	//输出格式化文件头
	xml_header(name, filename, 1);
	fout.open(filename, ios::app);
	fout << "<UserDefinedType>" << endl;
	//输出格式化非容器属性变量
	fout << "<idx>" << endl;
	fout.close();

	to_xml::serialize_xml(idx, name, filename, 0);

	fout.open(filename, ios::app);
	fout << "</idx>" << endl;
	fout << "<name>" << endl;
	fout.close();

	to_xml::serialize_xml(this->name, name, filename, 0);

	fout.open(filename, ios::app);
	fout << "</name>" << endl;
	fout.close();

	//输出类中容器变量
	to_xml_STL::serialize_xml_vector(data, name, filename, 0);

	fout.open(filename, ios::app);
	fout << "</UserDefinedType>" << endl;
	fout.close();
	//添加尾部格式化输出
	xml_tail(name, filename, 1);
}
//反序列化
void UserDefinedType::deserialize_myxml(std::string name, std::string filename, std::vector<int>& total_number)
{
	string quit_be;
	fin.open(filename, ios::in);
	//因为是第一次调用，需要去除文件头信息
	xml_quit(fin, 1);
	getline(fin, quit_be);
	double result;
	//读出非容器属性
	to_xml::get_xml(idx, fin, 1);
	to_xml::get_xml(this->name, fin, 1);

	getline(fin, quit_be);
	for (int index = 0;index < total_number[0];index++)
	{
		to_xml::get_xml(result, fin, 0);
		data.push_back(result);
	}
	getline(fin, quit_be);

	fin.close();
}