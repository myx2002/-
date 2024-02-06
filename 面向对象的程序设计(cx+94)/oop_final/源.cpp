#include<iostream>
#include<fstream>
#include<string>
#include<cmath>
#include<utility>
#include"type_binary.h"
#include"STL_binary.h"
#include"user_define_struct.h"
#include"basefunction_xml.h"
#include"type_xml.h"
#include"STL_xml.h"
using namespace std;
ifstream fin;
ofstream fout;

int main()
{
	//仅作为测试主程序
	//二进制序列化测试与xml文件序列化测试每次仅测试一类（变量相同命名）
	//二进制测试
	//一般类型
	//int
	int int_a = 66;
	int int_b = 1;
	cout << "int操作前：" << int_a << " != " << int_b << endl;
	to_binary::serialize_bi(int_a,"int_bi_test.txt", 0);
	to_binary::deserialize_bi(int_b, "int_bi_test.txt", 0);
	cout << "int操作后：" << int_a << " = " << int_b << endl;

	//bool
	bool bool_a = true;
	bool bool_b = false;
	cout << "bool操作前：" << bool_a << " != " << bool_b << endl;
	to_binary::serialize_bi(bool_a, "bool_bi_test.txt", 0);
	to_binary::deserialize_bi(bool_b, "bool_bi_test.txt", 0);
	cout << "bool操作后：" << bool_a << " = " << bool_b << endl;

	//char
	char char_a = 'a';
	char char_b = 'b';
	cout << "char操作前：" << char_a << " != " << char_b << endl;
	to_binary::serialize_bi(char_a, "char_bi_test.txt", 0);
	to_binary::deserialize_bi(char_b, "char_bi_test.txt", 0);
	cout << "char操作后：" << char_a << " = " << char_b << endl;

	//float
	float float_a = 1.23;
	float float_b = 4.56;
	cout << "float操作前：" << float_a << " != " << float_b << endl;
	to_binary::serialize_bi(float_a, "float_bi_test.txt", 0);
	to_binary::deserialize_bi(float_b, "float_bi_test.txt", 0);
	cout << "float操作后：" << float_a << " = " << float_b << endl;

	//double
	double double_a = 1.2345;
	double double_b = 6.7890;
	cout << "double操作前：" << double_a << " != " << double_b << endl;
	to_binary::serialize_bi(double_a, "double_bi_test.txt", 0);
	to_binary::deserialize_bi(double_b, "double_bi_test.txt", 0);
	cout << "double操作后：" << double_a << " = " << double_b << endl;

	//string
	string string_a = "abc";
	string string_b = "def";
	cout << "string操作前：" << string_a << " != " << string_b << endl;
	to_binary::serialize_bi(string_a, "string_bi_test.txt", 0);
	to_binary::deserialize_bi(string_b, "string_bi_test.txt", 0);
	cout << "string操作后：" << string_a << " = " << string_b << endl;


	//容器
	//pair
	pair<int, double> pair_a;
	pair<int, double> pair_b;
	pair_a.first = 10;
	pair_a.second = 1.11;
	pair_b.first = 0;
	pair_b.second = 0.01;
	cout << "操作前：" << pair_a.first << " != " << pair_b.first;
	cout << ", " << pair_a.second << " != " << pair_b.second << endl;
	to_binary_STL::serialize_pair(pair_a, "pair_bi_test1.txt", 0);
	to_binary_STL::deserialize_pair(pair_b, "pair_bi_test1.txt", 0);
	cout << "操作后：" << pair_a.first << " = " << pair_b.first;
	cout << ", " << pair_a.second << " = " << pair_b.second << endl;
	pair<float, string> pair_c;
	pair<float, string> pair_d;
	pair_c.first = 1.11;
	pair_c.second = "abc";
	pair_d.first = 0.01;
	pair_d.second = "def";
	cout << "操作前：" << pair_c.first << " != " << pair_d.first;
	cout << ", " << pair_c.second << " != " << pair_d.second << endl;
	to_binary_STL::serialize_pair(pair_c, "pair_bi_test2.txt", 0);
	to_binary_STL::deserialize_pair(pair_d, "pair_bi_test2.txt", 0);
	cout << "操作后：" << pair_c.first << " = " << pair_d.first;
	cout << ", " << pair_c.second << " = " << pair_d.second << endl;

	//map
	map<int, double> map_a;
	map<int, double> map_b;
	map_a[10] = 1.11;
	map_a[5] = 2.22;
	map_b[0] = 0.01;
	map_b[5] = 3.33;
	cout << "操作前：map_a != map_b " << endl;
	to_binary_STL::serialize_map(map_a, "map_bi_test1.txt", 0);
	to_binary_STL::deserialize_map(map_b, "map_bi_test1.txt", 0);
	cout << "操作后：" << map_a[10] << " = " << map_b[10];
	cout << ", " << map_a[5] << " = " << map_b[5] << endl;
	map<char, string> map_c;
	map<char, string> map_d;
	map_c['a'] = "aaaa";
	map_c['b'] = "bbbb";
	map_d['c'] = "cccc";
	map_d['b'] = "dddd";
	cout << "操作前：map_c != map_d " << endl;
	to_binary_STL::serialize_map(map_c, "map_bi_test2.txt", 0);
	to_binary_STL::deserialize_map(map_d, "map_bi_test2.txt", 0);
	cout << "操作后：" << map_c['a'] << " = " << map_d['a'];
	cout << ", " << map_c['b'] << " = " << map_d['b'] << endl;

	//set
	set<double> set_a;
	set<double> set_b;
	set_a.insert(1);
	set_a.insert(2);
	set_a.insert(3);
	set_b.insert(0);
	cout << "操作前：set_a != set_b " << endl;
	to_binary_STL::serialize_set(set_a, "set_bi_test1.txt", 0);
	to_binary_STL::deserialize_set(set_b, "set_bi_test1.txt", 0);
	auto index1 = set_a.begin();
	auto index2 = set_b.begin();
	cout << "操作后：";
	for (;index1 != set_a.end() && index2 != set_b.end();index1++, index2++)
		cout << *index1 << " = " << *index2 << " ";
	cout << endl;
	set<char> set_c;
	set<char> set_d;
	set_c.insert('a');
	set_c.insert('b');
	set_c.insert('c');
	set_d.insert('e');
	cout << "操作前：set_c != set_d " << endl;
	to_binary_STL::serialize_set(set_c, "set_bi_test2.txt", 0);
	to_binary_STL::deserialize_set(set_d, "set_bi_test2.txt", 0);
	auto index3 = set_c.begin();
	auto index4 = set_d.begin();
	cout << "操作后：";
	for (;index3 != set_c.end() && index4 != set_d.end();index3++, index4++)
		cout << *index3 << " = " << *index4 << " ";
	cout << endl;

	//vector
	vector<int> vector_a;
	vector<int> vector_b;
	vector_a.push_back(1);
	vector_a.push_back(2);
	vector_b.push_back(0);
	cout << "操作前：vector_a != vector_b " << endl;
	to_binary_STL::serialize_vector(vector_a, "vector_bi_test1.txt", 0);
	to_binary_STL::deserialize_vector(vector_b, "vector_bi_test1.txt", 0);
	auto index1_ve = vector_a.begin();
	auto index2_ve = vector_b.begin();
	cout << "操作后：";
	for (;index1_ve != vector_a.end() && index2_ve != vector_b.end();index1_ve++, index2_ve++)
	cout << *index1_ve << " = " << *index2_ve << " ";
	cout << endl;
	vector<bool> vector_c;
	vector<bool> vector_d;
	vector_c.push_back(1);
	vector_c.push_back(0);
	vector_d.push_back(0);
	cout << "操作前：vector_c != vector_d " << endl;
	to_binary_STL::serialize_vector(vector_c, "vector_bi_test2.txt", 0);
	to_binary_STL::deserialize_vector(vector_d, "vector_bi_test2.txt", 0);
	auto index3_ve = vector_c.begin();
	auto index4_ve = vector_d.begin();
	cout << "操作后：";
	for (;index3_ve != vector_c.end() && index4_ve != vector_d.end();index3_ve++, index4_ve++)
		cout << *index3_ve << " = " << *index4_ve << " ";
	cout << endl;

	//list
	list<float> list_a;
	list<float> list_b;
	list_a.push_back(1.1);
	list_a.push_back(2.2);
	list_b.push_back(0.1);
	cout << "操作前：list_a != list_b " << endl;
	to_binary_STL::serialize_list(list_a, "list_bi_test1.txt", 0);
	to_binary_STL::deserialize_list(list_b, "list_bi_test1.txt", 0);
	auto index1_li = list_a.begin();
	auto index2_li = list_b.begin();
	cout << "操作后：";
	for (;index1_li != list_a.end() && index2_li != list_b.end();index1_li++, index2_li++)
	cout << *index1_li << " = " << *index2_li << " ";
	cout << endl;
	list<string> list_c;
	list<string> list_d;
	list_c.push_back("abc");
	list_c.push_back("def");
	list_d.push_back("hg");
	cout << "操作前：list_c != list_d " << endl;
	to_binary_STL::serialize_list(list_c, "list_bi_test2.txt", 0);
	to_binary_STL::deserialize_list(list_d, "list_bi_test2.txt", 0);
	auto index3_li = list_c.begin();
	auto index4_li = list_d.begin();
	cout << "操作后：";
	for (;index3_li != list_c.end() && index4_li != list_d.end();index3_li++, index4_li++)
		cout << *index3_li << " = " << *index4_li << " ";
	cout << endl;

	//自定义class
	UserDefinedType class_a;
	UserDefinedType class_b;
	class_a.idx = 66;
	class_a.name = "meiyanxiao";
	cout << "操作前：class_a != class_b " << endl;
	class_a.serialize_bi("class_test.txt");
	class_b.deserialize_bi("class_test.txt");
	cout << "操作后：" << class_a.idx << " = " << class_b.idx << ", " << class_a.name << " = " << class_b.name << endl;		
	/*
	//xml文件转换测试
	//一般类型
	//int
	int int_a = 66;
	int int_b = 1;
	cout << "int操作前：" << int_a << " != " << int_b << endl;
	to_xml::serialize_xml(int_a, "first", "int_xml_test.xml", 1);
	to_xml::deserialize_xml(int_b, "first", "int_xml_test.xml", 1);
	cout << "int操作后：" << int_a << " = " << int_b << endl;

	//bool
	bool bool_a = true;
	bool bool_b = false;
	cout << "bool操作前：" << bool_a << " != " << bool_b << endl;
	to_xml::serialize_xml(bool_a, "first", "bool_xml_test.xml", 1);
	to_xml::deserialize_xml(bool_b, "first", "bool_xml_test.xml", 1);
	cout << "bool操作后：" << bool_a << " = " << bool_b << endl;

	//char
	char char_a = 'a';
	char char_b = 'b';
	cout << "char操作前：" << char_a << " != " << char_b << endl;
	to_xml::serialize_xml(char_a, "first", "char_xml_test.xml", 1);
	to_xml::deserialize_xml(char_b, "first", "char_xml_test.xml", 1);
	cout << "char操作后：" << char_a << " = " << char_b << endl;

	//float
	float float_a = 1.23;
	float float_b = 4.56;
	cout << "float操作前：" << float_a << " != " << float_b << endl;
	to_xml::serialize_xml(float_a, "first", "float_xml_test.xml", 1);
	to_xml::deserialize_xml(float_b, "first", "float_xml_test.xml", 1);
	cout << "float操作后：" << float_a << " = " << float_b << endl;

	//double
	double double_a = 1.2345;
	double double_b = 6.7890;
	cout << "double操作前：" << double_a << " != " << double_b << endl;
	to_xml::serialize_xml(double_a, "first", "double_xml_test.xml", 1);
	to_xml::deserialize_xml(double_b, "first", "double_xml_test.xml", 1);
	cout << "double操作后：" << double_a << " = " << double_b << endl;

	//string
	string string_a = "abc";
	string string_b = "def";
	cout << "string操作前：" << string_a << " != " << string_b << endl;
	to_xml::serialize_xml(string_a, "first", "string_xml_test.xml", 1);
	to_xml::deserialize_xml(string_b, "first", "string_xml_test.xml", 1);
	cout << "string操作后：" << string_a << " = " << string_b << endl;
	
	int result_number;
	//容器
	//pair
	pair<int, double> pair_a;
	pair<int, double> pair_b;
	pair_a.first = 10;
	pair_a.second = 1.11;
	pair_b.first = 0;
	pair_b.second = 0.01;
	cout << "操作前：" << pair_a.first << " != " << pair_b.first;
	cout << ", " << pair_a.second << " != " << pair_b.second << endl;
	result_number = to_xml_STL::serialize_xml_pair(pair_a, "first", "pair_xml_test1.xml", 1);
	to_xml_STL::deserialize_xml_pair(pair_b, result_number, "pair_xml_test1.xml", 1);
	cout << "操作后：" << pair_a.first << " = " << pair_b.first;
	cout << ", " << pair_a.second << " = " << pair_b.second << endl;
	pair<float, string> pair_c;
	pair<float, string> pair_d;
	pair_c.first = 1.11;
	pair_c.second = "abc";
	pair_d.first = 0.01;
	pair_d.second = "def";
	cout << "操作前：" << pair_c.first << " != " << pair_d.first;
	cout << ", " << pair_c.second << " != " << pair_d.second << endl;
	result_number = to_xml_STL::serialize_xml_pair(pair_c, "first", "pair_xml_test2.xml", 1);
	to_xml_STL::deserialize_xml_pair(pair_d, result_number, "pair_xml_test2.xml", 1);
	cout << "操作后：" << pair_c.first << " = " << pair_d.first;
	cout << ", " << pair_c.second << " = " << pair_d.second << endl;

	//map
	map<int, double> map_a;
	map<int, double> map_b;
	map_a[10] = 1.11;
	map_a[5] = 2.22;
	map_b[0] = 0.01;
	map_b[5] = 3.33;
	cout << "操作前：map_a != map_b " << endl;
	result_number = to_xml_STL::serialize_xml_map(map_a, "first", "map_xml_test1.xml", 1);
	to_xml_STL::deserialize_xml_map(map_b, result_number, "map_xml_test1.xml", 1);
	cout << "操作后：" << map_a[10] << " = " << map_b[10];
	cout << ", " << map_a[5] << " = " << map_b[5] << endl;
	map<char, string> map_c;
	map<char, string> map_d;
	map_c['a'] = "aaaa";
	map_c['b'] = "bbbb";
	map_d['c'] = "cccc";
	map_d['b'] = "dddd";
	cout << "操作前：map_c != map_d " << endl;
	result_number = to_xml_STL::serialize_xml_map(map_c, "first", "map_xml_test2.xml", 1);
	to_xml_STL::deserialize_xml_map(map_d, result_number, "map_xml_test2.xml", 1);
	cout << "操作后：" << map_c['a'] << " = " << map_d['a'];
	cout << ", " << map_c['b'] << " = " << map_d['b'] << endl;

	//set
	set<double> set_a;
	set<double> set_b;
	set_a.insert(1);
	set_a.insert(2);
	set_a.insert(3);
	set_b.insert(0);
	cout << "操作前：set_a != set_b " << endl;
	result_number = to_xml_STL::serialize_xml_set(set_a, "first", "set_xml_test1.xml", 1);
	to_xml_STL::deserialize_xml_set(set_b, result_number, "set_xml_test1.xml", 1);
	auto index1 = set_a.begin();
	auto index2 = set_b.begin();
	cout << "操作后：";
	for (;index1 != set_a.end() && index2 != set_b.end();index1++, index2++)
		cout << *index1 << " = " << *index2 << " ";
	cout << endl;
	set<char> set_c;
	set<char> set_d;
	set_c.insert('a');
	set_c.insert('b');
	set_c.insert('c');
	set_d.insert('e');
	cout << "操作前：set_c != set_d " << endl;
	result_number = to_xml_STL::serialize_xml_set(set_c, "first", "set_xml_test2.xml", 1);
	to_xml_STL::deserialize_xml_set(set_d, result_number, "set_xml_test2.xml", 1);
	auto index3 = set_c.begin();
	auto index4 = set_d.begin();
	cout << "操作后：";
	for (;index3 != set_c.end() && index4 != set_d.end();index3++, index4++)
		cout << *index3 << " = " << *index4 << " ";
	cout << endl;

	//vector
	vector<int> vector_a;
	vector<int> vector_b;
	vector_a.push_back(1);
	vector_a.push_back(2);
	vector_b.push_back(0);
	cout << "操作前：vector_a != vector_b " << endl;
	result_number = to_xml_STL::serialize_xml_vector(vector_a, "first", "vector_xml_test1.xml", 1);
	to_xml_STL::deserialize_xml_vector(vector_b, result_number, "vector_xml_test1.xml", 1);
	auto index1_ve = vector_a.begin();
	auto index2_ve = vector_b.begin();
	cout << "操作后：";
	for (;index1_ve != vector_a.end() && index2_ve != vector_b.end();index1_ve++, index2_ve++)
	cout << *index1_ve << " = " << *index2_ve << " ";
	cout << endl;
	vector<bool> vector_c;
	vector<bool> vector_d;
	vector_c.push_back(1);
	vector_c.push_back(0);
	vector_d.push_back(0);
	cout << "操作前：vector_c != vector_d " << endl;
	result_number = to_xml_STL::serialize_xml_vector(vector_c, "first", "vector_xml_test2.xml", 1);
	to_xml_STL::deserialize_xml_vector(vector_d, result_number, "vector_xml_test2.xml", 1);
	auto index3_ve = vector_c.begin();
	auto index4_ve = vector_d.begin();
	cout << "操作后：";
	for (;index3_ve != vector_c.end() && index4_ve != vector_d.end();index3_ve++, index4_ve++)
		cout << *index3_ve << " = " << *index4_ve << " ";
	cout << endl;

	//list
	list<float> list_a;
	list<float> list_b;
	list_a.push_back(1.1);
	list_a.push_back(2.2);
	list_b.push_back(0.1);
	cout << "操作前：list_a != list_b " << endl;
	result_number = to_xml_STL::serialize_xml_list(list_a, "first", "list_xml_test1.xml", 1);
	to_xml_STL::deserialize_xml_list(list_b, result_number, "list_xml_test1.xml", 1);
	auto index1_li = list_a.begin();
	auto index2_li = list_b.begin();
	cout << "操作后：";
	for (;index1_li != list_a.end() && index2_li != list_b.end();index1_li++, index2_li++)
	cout << *index1_li << " = " << *index2_li << " ";
	cout << endl;
	list<string> list_c;
	list<string> list_d;
	list_c.push_back("abc");
	list_c.push_back("def");
	list_d.push_back("hg");
	cout << "操作前：list_c != list_d " << endl;
	result_number = to_xml_STL::serialize_xml_list(list_c, "first", "list_xml_test2.xml", 1);
	to_xml_STL::deserialize_xml_list(list_d, result_number, "list_xml_test2.xml", 1);
	auto index3_li = list_c.begin();
	auto index4_li = list_d.begin();
	cout << "操作后：";
	for (;index3_li != list_c.end() && index4_li != list_d.end();index3_li++, index4_li++)
		cout << *index3_li << " = " << *index4_li << " ";
	cout << endl;
	//自定义class
	UserDefinedType class_a;
	UserDefinedType class_b;
	vector<int>record;
	class_a.idx = 66;
	class_a.name = "meiyanxiao";
	cout << "操作前：class_a != class_b " << endl;
	class_a.serialize_myxml("first", "class_test.xml",record);
	class_b.deserialize_myxml("first", "class_test.xml",record);
	cout << "操作后：" << class_a.idx << " = " << class_b.idx << ", " << class_a.name << " = " << class_b.name << endl;
	*/
	return 0;
}