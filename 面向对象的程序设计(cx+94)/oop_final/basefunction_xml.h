#pragma once
//---**ͨ�ú���**---
extern std::ifstream fin;
extern std::ofstream fout;
//xml��ʽ���ƺ���
void xml_format(std::ofstream& fout, int number);
//originΪ0��ֱ�ӷ��أ���ʾ�ڲ�����
//originΪ1�����ʾ�ⲿ��������Ҫȥ��header_file
//ͷ��Ϣ
void xml_header(std::string name, std::string filename, int origin);
//β��Ϣ
void xml_tail(std::string name, std::string filename, int origin);
//ȥ��ָ��������Ϣ����ͷ��ʼ��
void xml_quit(std::ifstream& fin, int origin);
//����int/double/float�����ж��Ƿ�Ϊ��
int check_neg(std::string receive, int& begin_num);