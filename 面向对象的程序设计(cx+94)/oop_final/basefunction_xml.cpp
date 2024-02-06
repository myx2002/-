//---**xml文件转换通用函数**---
#include<iostream>
#include<fstream>
#include<string>
#include"basefunction_xml.h"
using namespace std;

//xml格式控制函数
void xml_format(ofstream& fout, int number)
{
	//主要是控制空格的输出个数
	for (int index = 0;index < number - 1;index++)
		fout << " ";
}
//origin为0，直接返回，表示内部操作
//头信息输出
void xml_header(string name, string filename, int origin)
{
	//为0不去除头信息
	if (origin == 0)
		return;
	//格式化输出头信息
	fout.open(filename, ios::app);
	fout << "<serialization>" << endl;
	xml_format(fout, 4);
	fout << "<" << name << ">" << endl;
	fout.close();
}
//尾信息输出
void xml_tail(string name, string filename, int origin)
{
	if (origin == 0)
		return;
	fout.open(filename, ios::app);
	//格式化尾信息输出
	xml_format(fout, 4);
	fout << "</" << name << ">" << endl;
	fout << "</serialization>";
	fout.close();
}
//去除指定行数的信息
void xml_quit(ifstream& fin, int origin)
{
	if (origin == 0)
		return;
	string quit;
	//通过空读行不操作等效于去除指定行数信息操作
	for (int index = 0;index < 2;index++)
		getline(fin, quit);
}
//数值正负判断函数
int check_neg(string receive, int& begin_num)
{
	//根据记录的“-”有无进行正负判断
	if (receive[begin_num] == '-')
	{
		begin_num++;
		return 1;
	}
	return 0;
}