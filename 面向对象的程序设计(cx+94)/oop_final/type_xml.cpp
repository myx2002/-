#include<iostream>
#include<fstream>
#include<string>
#include"type_xml.h"
#include"basefunction_xml.h"
#define off 8
using namespace std;

//---**一般数据类型转变为xml结构**---
//int转换为xml
//origin作为1,需要插入头信息与标签；为0不需要处理头信息
//numcheck为1表示需要进行头控制输出
string quit;
void to_xml::get_xml(int& result, ifstream& fin, int origin)
{
	string receive;
	int begin_num;
	int flag = 0;
	result = 0;
	//根据调用的内外部来源，处理头信息
	if (origin == 1)
		getline(fin, quit);
	//整行读入进行对映操作
	getline(fin, receive);
	//直接根据“val=”结构进行一定的偏移
	begin_num = receive.find("val=") + 5;
	//判断整数是否为-
	flag = check_neg(receive, begin_num);
	while (receive[begin_num] != '\"')
	{
		//常规的将字符串转化为整数步骤
		result = result * 10 + int(receive[begin_num] - '0');
		begin_num++;
	}
	//若整数位-，需要进行取负操作
	if (flag == 1)
		result = -result;
	if (origin == 1)
		getline(fin, quit);
}
//序列化函数
void to_xml::serialize_xml(int int_type, string name, string filename, int origin)
{
	//填入头信息
	xml_header(name, filename, origin);

	fout.open(filename, ios::app);
	if (origin == 1)
		fout << "<i>" << endl;
	
	//按格式输出int类型对映的输出条例
	fout << "<" << "int val=\"" << int_type << "\"/>";
	fout << endl;
	if (origin == 1)
		fout << "</i>" << endl;

	fout.close();
	//填入尾信息
	xml_tail(name, filename, origin);
}
//反序列化函数
void to_xml::deserialize_xml(int& int_type, string name, string filename, int origin)
{
	
	fin.open(filename, ios::in);
	//处理头信息
	xml_quit(fin, origin);
	//直接调用核心函数即可
	get_xml(int_type, fin, origin);
	fin.close();
}

//bool转化为xml
void to_xml::get_xml(bool& result, ifstream& fin, int origin)
{
	string receive;
	int begin_num;
	//根据调用的内外部来源，处理头信息
	if (origin == 1)
		getline(fin, quit);
	//读取整行数据
	getline(fin, receive);
	//重定位有效信息
	begin_num = receive.find("val=") + 5;
	//因为是bool类型，所以直接返回即可
	result = bool(receive[begin_num] - '0');
	if (origin == 1)
		getline(fin, quit);
}
//序列化函数
//使用1代替true；0代替false
void to_xml::serialize_xml(bool bool_type, string name, string filename, int origin)
{
	//填入头信息
	xml_header(name, filename, origin);
	
	//填入头信息
	fout.open(filename, ios::app);
	if (origin == 1)
		fout << "<b>" << endl;
	//格式化输出bool格式对映的行记录
	fout << "<" << "bool val=\"" << bool_type << "\"/>";
	fout << endl;
	if (origin == 1)
		fout << "</b>" << endl;

	fout.close();
	//填入尾信息
	xml_tail(name, filename, origin);
}
//反序列化函数
void to_xml::deserialize_xml(bool& bool_type, string name, string filename, int origin)
{
	
	fin.open(filename, ios::in);
	//处理头信息
	xml_quit(fin, origin);
	//直接调用核心函数即可
	get_xml(bool_type, fin, origin);
	fin.close();
}

//char转化为xml
void to_xml::get_xml(char& result, ifstream& fin, int origin)
{
	string receive;
	int begin_num;
	//判断是否丢弃头信息
	if (origin == 1)
		getline(fin, quit);
	//获得一行数据
	getline(fin, receive);
	begin_num = receive.find("val=") + 5;
	//char类型直接返回
	result = receive[begin_num];
	if (origin == 1)
		getline(fin, quit);
}
//序列化函数
void to_xml::serialize_xml(char char_type, string name, string filename, int origin)
{
	//填入头信息
	xml_header(name, filename, origin);

	//填入头信息
	fout.open(filename, ios::app);
	if (origin == 1)
		fout << "<c>" << endl;
	//格式化输出char类型对映行记录
	xml_format(fout, off+origin);
	fout << "<" << "char val=\"" << char_type << "\"/>";
	fout << endl;
	if (origin == 1)
		fout << "</c>" << endl;

	fout.close();
	//填入尾信息
	xml_tail(name, filename, origin);
}
//反序列化函数
void to_xml::deserialize_xml(char& char_type, string name, string filename, int origin)
{
	
	fin.open(filename, ios::in);
	//处理头信息
	xml_quit(fin, origin);
	//直接调用核心函数即可
	get_xml(char_type, fin, origin);
	fin.close();
}

//float转化为xml
void to_xml::get_xml(float& result, ifstream& fin, int origin)
{
	string receive;
	int begin_num;
	//定义整数部分大小
	int int_num = 0;
	//定义小数部分数值大小
	int float_num = 0;
	//定义相应需要缩小的指数倍数
	int power_num = 0;
	int flag = 0;
	result = 0;
	//判断头信息的是否处理
	if (origin == 1)
		getline(fin, quit);
	getline(fin, receive);
	//更新起始位置
	begin_num = receive.find("val=") + 5;
	//float类型需要判断是否为负数
	flag = check_neg(receive, begin_num);
	//先根据“.”处理整数部分
	while (receive[begin_num] != '.')
	{
		//可能存在没有.的情况（整数输入，因而每次得到数据后都需要进行判断是否跨过.直接得到\"）
		if (receive[begin_num] == '\"')
			break;
		//常规转化
		int_num = int_num * 10 + int(receive[begin_num] - '0');
		begin_num++;
	}
	//不为\"时需要自增begin_num使得略过.
	if (receive[begin_num] != '\"')
		begin_num++;
	//按照整数方法激记录小数部分的数值
	while (receive[begin_num] != '\"')
	{
		float_num = float_num * 10 + int(receive[begin_num] - '0');
		power_num++;
		//更新放大的倍数
		begin_num++;
	}
	//缩小小数部分数值
	result = 1.0 * float_num / pow(10, power_num);
	//计算最后得到的数值并进行正负校正
	result = result + int_num;
	if (flag == 1)
		result = -result;
	if (origin == 1)
		getline(fin, quit);
}
//序列化函数
void to_xml::serialize_xml(float float_type, string name, string filename, int origin)
{
	//填入头信息
	xml_header(name, filename, origin);
	//填入头部信息
	fout.open(filename, ios::app);
	if (origin == 1)
		fout << "<f>" << endl;
	//格式化输出float类型对映的行记录
	xml_format(fout, off+origin);
	fout << "<" << "float val=\"" << float_type << "\"/>";
	fout << endl;
	if (origin == 1)
		fout << "</f>" << endl;
	
	fout.close();
	//填入尾信息
	xml_tail(name, filename, origin);
}
//反序列化函数
void to_xml::deserialize_xml(float& float_type, string name, string filename, int origin)
{
	fin.open(filename, ios::in);
	//处理头信息
	xml_quit(fin, origin);
	//直接调用核心函数即可
	get_xml(float_type, fin, origin);
	fin.close();
}

//double转化为xml
void to_xml::get_xml(double& result, ifstream& fin, int origin)
{
	//基本与float类型相同
	string receive;
	int begin_num;
	int int_num = 0;
	//定义整数部分与小数部分
	int double_num = 0;
	int power_num = 0;
	int flag = 0;
	result = 0;
	//处理头信息
	if (origin == 1)
		getline(fin, quit);
	//读取数据
	getline(fin, receive);
	//获得起始位置
	begin_num = receive.find("val=") + 5;
	//判断double类型正负
	flag = check_neg(receive, begin_num);
	while (receive[begin_num] != '.')
	{
		//同样需要预防只出现\"作为终止的情况
		if (receive[begin_num] == '\"')
			break;
		//常规转化字符串为数字
		int_num = int_num * 10 + int(receive[begin_num] - '0');
		begin_num++;
	}
	//根据.的有无对begin_num进行操作
	if (receive[begin_num] != '\"')
		begin_num++;
	//处理小数部分数据
	while (receive[begin_num] != '\"')
	{
		double_num = double_num * 10 + int(receive[begin_num] - '0');
		power_num++;
		//记录放大倍数
		begin_num++;
	}
	//对数值进行相应的缩小
	result = 1.0 * double_num / pow(10, power_num);
	result = result + int_num;
	//校正正负
	if (flag == 1)
		result = -result;
	if (origin == 1)
		getline(fin, quit);
}
//序列化函数
void to_xml::serialize_xml(double double_type, string name, string filename, int origin)
{
	///填入头信息
	xml_header(name, filename, origin);

	//填入头信息
	fout.open(filename, ios::app);
	if (origin == 1)
		fout << "<d>" << endl;
	//格式化输出double类型的记录行数据
	fout << "<" << "double val=\"" << double_type << "\"/>";
	fout << endl;
	if (origin == 1) 
		fout << "</d>" << endl;
	
	fout.close();
	//填入尾信息
	xml_tail(name, filename, origin);
}
//反序列化函数
void to_xml::deserialize_xml(double& double_type, string name, string filename, int origin)
{
	
	fin.open(filename, ios::in);
	//处理头信息
	xml_quit(fin, origin);
	//直接调用核心函数即可
	get_xml(double_type, fin, origin);
	fin.close();
}

//string转化为xml
void to_xml::get_xml(string& result, ifstream& fin, int origin)
{
	string receive;
	result = "";
	int begin_num;
	//处理头信息
	if (origin == 1)
		getline(fin, quit);
	//获取行数据
	getline(fin, receive);
	//更新begin_num数值
	begin_num = receive.find("val=") + 5;
	while (receive[begin_num] != '\"')
	{
		//sting类型直接不断叠加读出的char元素即可
		result += receive[begin_num];
		begin_num++;
	}
	if (origin == 1)
		getline(fin, quit);
}
//序列化函数
void to_xml::serialize_xml(string string_type, string name, string filename, int origin)
{
	//填入头信息
	xml_header(name, filename, origin);

	fout.open(filename, ios::app);
	if (origin == 1)
		fout << "<s>" << endl;
	//格式化输出string类型对应的行数据
	fout << "<" << "string val=\"" << string_type << "\"/>";
	fout << endl;
	if (origin == 1)
		fout << "</s>" << endl;

	fout.close();
	//填入尾信息
	xml_tail(name, filename, origin);
}
//反序列化函数
void to_xml::deserialize_xml(string& string_type, string name, string filename, int origin)
{
	
	fin.open(filename, ios::in);
	//处理头信息
	xml_quit(fin, origin);
	//直接调用核心函数即可
	get_xml(string_type, fin, origin);
	fin.close();
}