#include<iostream>
#include<fstream>
#include<string>
#include"type_xml.h"
#include"basefunction_xml.h"
#define off 8
using namespace std;

//---**һ����������ת��Ϊxml�ṹ**---
//intת��Ϊxml
//origin��Ϊ1,��Ҫ����ͷ��Ϣ���ǩ��Ϊ0����Ҫ����ͷ��Ϣ
//numcheckΪ1��ʾ��Ҫ����ͷ�������
string quit;
void to_xml::get_xml(int& result, ifstream& fin, int origin)
{
	string receive;
	int begin_num;
	int flag = 0;
	result = 0;
	//���ݵ��õ����ⲿ��Դ������ͷ��Ϣ
	if (origin == 1)
		getline(fin, quit);
	//���ж�����ж�ӳ����
	getline(fin, receive);
	//ֱ�Ӹ��ݡ�val=���ṹ����һ����ƫ��
	begin_num = receive.find("val=") + 5;
	//�ж������Ƿ�Ϊ-
	flag = check_neg(receive, begin_num);
	while (receive[begin_num] != '\"')
	{
		//����Ľ��ַ���ת��Ϊ��������
		result = result * 10 + int(receive[begin_num] - '0');
		begin_num++;
	}
	//������λ-����Ҫ����ȡ������
	if (flag == 1)
		result = -result;
	if (origin == 1)
		getline(fin, quit);
}
//���л�����
void to_xml::serialize_xml(int int_type, string name, string filename, int origin)
{
	//����ͷ��Ϣ
	xml_header(name, filename, origin);

	fout.open(filename, ios::app);
	if (origin == 1)
		fout << "<i>" << endl;
	
	//����ʽ���int���Ͷ�ӳ���������
	fout << "<" << "int val=\"" << int_type << "\"/>";
	fout << endl;
	if (origin == 1)
		fout << "</i>" << endl;

	fout.close();
	//����β��Ϣ
	xml_tail(name, filename, origin);
}
//�����л�����
void to_xml::deserialize_xml(int& int_type, string name, string filename, int origin)
{
	
	fin.open(filename, ios::in);
	//����ͷ��Ϣ
	xml_quit(fin, origin);
	//ֱ�ӵ��ú��ĺ�������
	get_xml(int_type, fin, origin);
	fin.close();
}

//boolת��Ϊxml
void to_xml::get_xml(bool& result, ifstream& fin, int origin)
{
	string receive;
	int begin_num;
	//���ݵ��õ����ⲿ��Դ������ͷ��Ϣ
	if (origin == 1)
		getline(fin, quit);
	//��ȡ��������
	getline(fin, receive);
	//�ض�λ��Ч��Ϣ
	begin_num = receive.find("val=") + 5;
	//��Ϊ��bool���ͣ�����ֱ�ӷ��ؼ���
	result = bool(receive[begin_num] - '0');
	if (origin == 1)
		getline(fin, quit);
}
//���л�����
//ʹ��1����true��0����false
void to_xml::serialize_xml(bool bool_type, string name, string filename, int origin)
{
	//����ͷ��Ϣ
	xml_header(name, filename, origin);
	
	//����ͷ��Ϣ
	fout.open(filename, ios::app);
	if (origin == 1)
		fout << "<b>" << endl;
	//��ʽ�����bool��ʽ��ӳ���м�¼
	fout << "<" << "bool val=\"" << bool_type << "\"/>";
	fout << endl;
	if (origin == 1)
		fout << "</b>" << endl;

	fout.close();
	//����β��Ϣ
	xml_tail(name, filename, origin);
}
//�����л�����
void to_xml::deserialize_xml(bool& bool_type, string name, string filename, int origin)
{
	
	fin.open(filename, ios::in);
	//����ͷ��Ϣ
	xml_quit(fin, origin);
	//ֱ�ӵ��ú��ĺ�������
	get_xml(bool_type, fin, origin);
	fin.close();
}

//charת��Ϊxml
void to_xml::get_xml(char& result, ifstream& fin, int origin)
{
	string receive;
	int begin_num;
	//�ж��Ƿ���ͷ��Ϣ
	if (origin == 1)
		getline(fin, quit);
	//���һ������
	getline(fin, receive);
	begin_num = receive.find("val=") + 5;
	//char����ֱ�ӷ���
	result = receive[begin_num];
	if (origin == 1)
		getline(fin, quit);
}
//���л�����
void to_xml::serialize_xml(char char_type, string name, string filename, int origin)
{
	//����ͷ��Ϣ
	xml_header(name, filename, origin);

	//����ͷ��Ϣ
	fout.open(filename, ios::app);
	if (origin == 1)
		fout << "<c>" << endl;
	//��ʽ�����char���Ͷ�ӳ�м�¼
	xml_format(fout, off+origin);
	fout << "<" << "char val=\"" << char_type << "\"/>";
	fout << endl;
	if (origin == 1)
		fout << "</c>" << endl;

	fout.close();
	//����β��Ϣ
	xml_tail(name, filename, origin);
}
//�����л�����
void to_xml::deserialize_xml(char& char_type, string name, string filename, int origin)
{
	
	fin.open(filename, ios::in);
	//����ͷ��Ϣ
	xml_quit(fin, origin);
	//ֱ�ӵ��ú��ĺ�������
	get_xml(char_type, fin, origin);
	fin.close();
}

//floatת��Ϊxml
void to_xml::get_xml(float& result, ifstream& fin, int origin)
{
	string receive;
	int begin_num;
	//�����������ִ�С
	int int_num = 0;
	//����С��������ֵ��С
	int float_num = 0;
	//������Ӧ��Ҫ��С��ָ������
	int power_num = 0;
	int flag = 0;
	result = 0;
	//�ж�ͷ��Ϣ���Ƿ���
	if (origin == 1)
		getline(fin, quit);
	getline(fin, receive);
	//������ʼλ��
	begin_num = receive.find("val=") + 5;
	//float������Ҫ�ж��Ƿ�Ϊ����
	flag = check_neg(receive, begin_num);
	//�ȸ��ݡ�.��������������
	while (receive[begin_num] != '.')
	{
		//���ܴ���û��.��������������룬���ÿ�εõ����ݺ���Ҫ�����ж��Ƿ���.ֱ�ӵõ�\"��
		if (receive[begin_num] == '\"')
			break;
		//����ת��
		int_num = int_num * 10 + int(receive[begin_num] - '0');
		begin_num++;
	}
	//��Ϊ\"ʱ��Ҫ����begin_numʹ���Թ�.
	if (receive[begin_num] != '\"')
		begin_num++;
	//����������������¼С�����ֵ���ֵ
	while (receive[begin_num] != '\"')
	{
		float_num = float_num * 10 + int(receive[begin_num] - '0');
		power_num++;
		//���·Ŵ�ı���
		begin_num++;
	}
	//��СС��������ֵ
	result = 1.0 * float_num / pow(10, power_num);
	//�������õ�����ֵ����������У��
	result = result + int_num;
	if (flag == 1)
		result = -result;
	if (origin == 1)
		getline(fin, quit);
}
//���л�����
void to_xml::serialize_xml(float float_type, string name, string filename, int origin)
{
	//����ͷ��Ϣ
	xml_header(name, filename, origin);
	//����ͷ����Ϣ
	fout.open(filename, ios::app);
	if (origin == 1)
		fout << "<f>" << endl;
	//��ʽ�����float���Ͷ�ӳ���м�¼
	xml_format(fout, off+origin);
	fout << "<" << "float val=\"" << float_type << "\"/>";
	fout << endl;
	if (origin == 1)
		fout << "</f>" << endl;
	
	fout.close();
	//����β��Ϣ
	xml_tail(name, filename, origin);
}
//�����л�����
void to_xml::deserialize_xml(float& float_type, string name, string filename, int origin)
{
	fin.open(filename, ios::in);
	//����ͷ��Ϣ
	xml_quit(fin, origin);
	//ֱ�ӵ��ú��ĺ�������
	get_xml(float_type, fin, origin);
	fin.close();
}

//doubleת��Ϊxml
void to_xml::get_xml(double& result, ifstream& fin, int origin)
{
	//������float������ͬ
	string receive;
	int begin_num;
	int int_num = 0;
	//��������������С������
	int double_num = 0;
	int power_num = 0;
	int flag = 0;
	result = 0;
	//����ͷ��Ϣ
	if (origin == 1)
		getline(fin, quit);
	//��ȡ����
	getline(fin, receive);
	//�����ʼλ��
	begin_num = receive.find("val=") + 5;
	//�ж�double��������
	flag = check_neg(receive, begin_num);
	while (receive[begin_num] != '.')
	{
		//ͬ����ҪԤ��ֻ����\"��Ϊ��ֹ�����
		if (receive[begin_num] == '\"')
			break;
		//����ת���ַ���Ϊ����
		int_num = int_num * 10 + int(receive[begin_num] - '0');
		begin_num++;
	}
	//����.�����޶�begin_num���в���
	if (receive[begin_num] != '\"')
		begin_num++;
	//����С����������
	while (receive[begin_num] != '\"')
	{
		double_num = double_num * 10 + int(receive[begin_num] - '0');
		power_num++;
		//��¼�Ŵ���
		begin_num++;
	}
	//����ֵ������Ӧ����С
	result = 1.0 * double_num / pow(10, power_num);
	result = result + int_num;
	//У������
	if (flag == 1)
		result = -result;
	if (origin == 1)
		getline(fin, quit);
}
//���л�����
void to_xml::serialize_xml(double double_type, string name, string filename, int origin)
{
	///����ͷ��Ϣ
	xml_header(name, filename, origin);

	//����ͷ��Ϣ
	fout.open(filename, ios::app);
	if (origin == 1)
		fout << "<d>" << endl;
	//��ʽ�����double���͵ļ�¼������
	fout << "<" << "double val=\"" << double_type << "\"/>";
	fout << endl;
	if (origin == 1) 
		fout << "</d>" << endl;
	
	fout.close();
	//����β��Ϣ
	xml_tail(name, filename, origin);
}
//�����л�����
void to_xml::deserialize_xml(double& double_type, string name, string filename, int origin)
{
	
	fin.open(filename, ios::in);
	//����ͷ��Ϣ
	xml_quit(fin, origin);
	//ֱ�ӵ��ú��ĺ�������
	get_xml(double_type, fin, origin);
	fin.close();
}

//stringת��Ϊxml
void to_xml::get_xml(string& result, ifstream& fin, int origin)
{
	string receive;
	result = "";
	int begin_num;
	//����ͷ��Ϣ
	if (origin == 1)
		getline(fin, quit);
	//��ȡ������
	getline(fin, receive);
	//����begin_num��ֵ
	begin_num = receive.find("val=") + 5;
	while (receive[begin_num] != '\"')
	{
		//sting����ֱ�Ӳ��ϵ��Ӷ�����charԪ�ؼ���
		result += receive[begin_num];
		begin_num++;
	}
	if (origin == 1)
		getline(fin, quit);
}
//���л�����
void to_xml::serialize_xml(string string_type, string name, string filename, int origin)
{
	//����ͷ��Ϣ
	xml_header(name, filename, origin);

	fout.open(filename, ios::app);
	if (origin == 1)
		fout << "<s>" << endl;
	//��ʽ�����string���Ͷ�Ӧ��������
	fout << "<" << "string val=\"" << string_type << "\"/>";
	fout << endl;
	if (origin == 1)
		fout << "</s>" << endl;

	fout.close();
	//����β��Ϣ
	xml_tail(name, filename, origin);
}
//�����л�����
void to_xml::deserialize_xml(string& string_type, string name, string filename, int origin)
{
	
	fin.open(filename, ios::in);
	//����ͷ��Ϣ
	xml_quit(fin, origin);
	//ֱ�ӵ��ú��ĺ�������
	get_xml(string_type, fin, origin);
	fin.close();
}