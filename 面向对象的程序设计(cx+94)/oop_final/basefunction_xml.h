#pragma once
//---**通用函数**---
extern std::ifstream fin;
extern std::ofstream fout;
//xml格式控制函数
void xml_format(std::ofstream& fout, int number);
//origin为0，直接返回，表示内部操作
//origin为1，则表示外部操作，需要去除header_file
//头信息
void xml_header(std::string name, std::string filename, int origin);
//尾信息
void xml_tail(std::string name, std::string filename, int origin);
//去除指定行数信息（从头开始）
void xml_quit(std::ifstream& fin, int origin);
//对于int/double/float类型判断是否为负
int check_neg(std::string receive, int& begin_num);