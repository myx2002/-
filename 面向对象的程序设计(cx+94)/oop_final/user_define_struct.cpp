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
//ʵ���Զ���ģ���serialize��deserialize����
//���������л�
int UserDefinedType::serialize_bi(string filename)
{
	//��Ҫ����һƫ����
	int offset = 0;
	//ֻ��Ҫ������е�Ԫ�ص���serialize_xxx��������
	//�ر�ע�⣬����һ�����ͣ�ʹ��to_binary�ռ�ĺ�������serialize_bi��������
	//			�����������ͣ���Ҫʹ��to_binary_STL�ռ�ĺ�������serialize_��������������
	//ÿ�κ������ú�������Ҫ����ƫ����
	offset += to_binary::serialize_bi(idx, filename, 0);
	offset += to_binary::serialize_bi(name, filename, 0);
	offset += to_binary_STL::serialize_vector(data, filename, 0);
	//��󷵻�ƫ��������
	return offset;
}
//�����Ʒ����л�
int UserDefinedType::deserialize_bi(string filename)
{
	//��Ҫ����һƫ����
	int offset = 0;
	//ͬ�������л�������ֻ����ö�ӳnamespace��deserialize_xxxx����(����ͬ���л�������
	offset += to_binary::deserialize_bi(idx, filename, offset);
	offset += to_binary::deserialize_bi(name, filename, offset);
	offset += to_binary_STL::deserialize_vector(data, filename, offset);
	//���ϸ���ƫ���������ؼ���
	return offset;
}
//xml�ļ����л�
//�����ڶ�����ת����xml�ļ���ʽ��Ҫ��¼���Ӧ���������ƣ������Ҫ���ӵ����⻯���
void UserDefinedType::serialize_myxml(std::string name, std::string filename, std::vector<int>& total_number)
{
	//��¼�������Դ�С�������������з�����ͳһ
	total_number.push_back(data.size());
	//�����ʽ���ļ�ͷ
	xml_header(name, filename, 1);
	fout.open(filename, ios::app);
	fout << "<UserDefinedType>" << endl;
	//�����ʽ�����������Ա���
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

	//���������������
	to_xml_STL::serialize_xml_vector(data, name, filename, 0);

	fout.open(filename, ios::app);
	fout << "</UserDefinedType>" << endl;
	fout.close();
	//���β����ʽ�����
	xml_tail(name, filename, 1);
}
//�����л�
void UserDefinedType::deserialize_myxml(std::string name, std::string filename, std::vector<int>& total_number)
{
	string quit_be;
	fin.open(filename, ios::in);
	//��Ϊ�ǵ�һ�ε��ã���Ҫȥ���ļ�ͷ��Ϣ
	xml_quit(fin, 1);
	getline(fin, quit_be);
	double result;
	//��������������
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