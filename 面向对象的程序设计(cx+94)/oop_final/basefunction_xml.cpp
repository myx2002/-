//---**xml�ļ�ת��ͨ�ú���**---
#include<iostream>
#include<fstream>
#include<string>
#include"basefunction_xml.h"
using namespace std;

//xml��ʽ���ƺ���
void xml_format(ofstream& fout, int number)
{
	//��Ҫ�ǿ��ƿո���������
	for (int index = 0;index < number - 1;index++)
		fout << " ";
}
//originΪ0��ֱ�ӷ��أ���ʾ�ڲ�����
//ͷ��Ϣ���
void xml_header(string name, string filename, int origin)
{
	//Ϊ0��ȥ��ͷ��Ϣ
	if (origin == 0)
		return;
	//��ʽ�����ͷ��Ϣ
	fout.open(filename, ios::app);
	fout << "<serialization>" << endl;
	xml_format(fout, 4);
	fout << "<" << name << ">" << endl;
	fout.close();
}
//β��Ϣ���
void xml_tail(string name, string filename, int origin)
{
	if (origin == 0)
		return;
	fout.open(filename, ios::app);
	//��ʽ��β��Ϣ���
	xml_format(fout, 4);
	fout << "</" << name << ">" << endl;
	fout << "</serialization>";
	fout.close();
}
//ȥ��ָ����������Ϣ
void xml_quit(ifstream& fin, int origin)
{
	if (origin == 0)
		return;
	string quit;
	//ͨ���ն��в�������Ч��ȥ��ָ��������Ϣ����
	for (int index = 0;index < 2;index++)
		getline(fin, quit);
}
//��ֵ�����жϺ���
int check_neg(string receive, int& begin_num)
{
	//���ݼ�¼�ġ�-�����޽��������ж�
	if (receive[begin_num] == '-')
	{
		begin_num++;
		return 1;
	}
	return 0;
}