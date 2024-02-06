#pragma once
#include<set>
#include<map>
#include<string>
#include<vector>
#include<list>
#include"type_xml.h"
#include"basefunction_xml.h"
#define off 8
extern std::ifstream fin;
extern std::ofstream fout;
using namespace std;
//需不需要std::导引

//---**容器转变为xml结构**---
// 统一输出格式说明
// 1.输入参数类型
// 2.文件标识名
// 3.文件名
// 4.是否为主写入调用（用户使用默认为1）
//pair转变为xml
namespace to_xml_STL
{
	//pair类型转变xml文件
	//序列化函数
	template<typename T, typename I>
	int serialize_xml_pair(std::pair<T, I> pair_type, std::string name, std::string filename, int origin)
	{
		//填入头信息
		xml_header(name, filename, origin);

		fout.open(filename, ios::app);
		xml_format(fout, 0);
		fout << "<pair>" << endl;
		//格式化写入pair对映的first属性行数据
		xml_format(fout, 0);
		fout << "<first>" << endl;
		fout.close();

		to_xml::serialize_xml(pair_type.first, name, filename, 0);

		fout.open(filename, ios::app);
		xml_format(fout, 0);
		fout << "</first>" << endl;
		//格式化写入pair对映的second属性行数据
		xml_format(fout, 0);
		fout << "<second>" << endl;
		fout.close();

		to_xml::serialize_xml(pair_type.second, name, filename, 0);

		fout.open(filename, ios::app);
		xml_format(fout, 0);
		fout << "</second>" << endl;
		xml_format(fout, 0);
		fout << "</pair>" << endl;
		fout.close();

		//填入尾信息
		xml_tail(name, filename, origin);

		return 2;
	}
	//反序列函数
	template<typename T, typename I>
	void deserialize_xml_pair(std::pair<T, I>& pair_type, int total_num, std::string filename, int origin)
	{
		string quit_be;
		fin.open(filename, ios::in);
		//处理头信息
		xml_quit(fin, origin);
		//处理头信息
		getline(fin, quit_be);
		//按对映的数据类型读取pair对映的first/second属性数据
		to_xml::get_xml(pair_type.first, fin, 1);
		to_xml::get_xml(pair_type.second, fin, 1);
		getline(fin, quit_be);
		fin.close();
	}


	//map类型转变xml文件
	//序列化函数
	template<typename T, typename I>
	int serialize_xml_map(std::map<T, I> map_type, std::string name, std::string filename, int origin)
	{
		//填入头信息
		xml_header(name, filename, origin);
		
		int number = 0;
		//填入头信息
		fout.open(filename, ios::app);
		fout << "<map>" << endl;
		//先获得map容器对映的大小并写入文件，便于后续单行对象的逐一写入
		int total_num = map_type.size();
		//格式化写入size整型
		fout.close();

		for (auto index = map_type.begin();index != map_type.end();index++)
		{
			serialize_xml_pair(*index, name, filename, 0);
			number++;
		}

		fout.open(filename, ios::app);
		xml_format(fout, origin);
		fout << "</map>" << endl;
		fout.close();

		//填入尾信息
		xml_tail(name, filename, origin);

		return total_num;
	}
	//反序列函数
	template<typename T, typename I>
	void deserialize_xml_map(std::map<T, I>& map_type, int total_num, std::string filename, int origin)
	{
		string quit_be;
		map_type.clear();
		fin.open(filename, ios::in);
		//处理头信息
		xml_quit(fin, origin);
		getline(fin, quit_be);
		//处理头信息
		T result_one;
		I result_two;
		//根据容器大小，获得map各行对象对映的first/second属性
		for (int index = 0;index < total_num;index++)
		{	
			getline(fin, quit_be);
			to_xml::get_xml(result_one, fin, 1);
			to_xml::get_xml(result_two, fin, 1);
			//对于新map对象，需要将first属性与second属性做一一对应
			map_type[result_one] = result_two;
			getline(fin, quit_be);
		}
		getline(fin, quit_be);
		fin.close();
	}

	//set类型转变xml文件
	//序列化函数
	template<typename T>
	int serialize_xml_set(std::set<T> set_type, std::string name, std::string filename, int origin)
	{
		//处理头信息
		xml_header(name, filename, origin);

		fout.open(filename, ios::app);
		fout << "<set>" << endl;
		//先获得set容器对映的大小并写入文件，便于后续单行对象的逐一写入
		int total_num = set_type.size();
		//根据总容器个数一一写入set元素
		fout.close();
		for (auto index = set_type.begin();index != set_type.end();index++)
			to_xml::serialize_xml(*index, name, filename, 0);

		fout.open(filename, ios::app);
		fout << "</set>" << endl;
		fout.close();
		//填入尾信息
		xml_tail(name, filename, origin);
		return total_num;
	}
	//反序列函数
	template<typename T>
	void deserialize_xml_set(std::set<T>& set_type, int total_num, std::string filename, int origin)
	{
		string quit_be;
		set_type.clear();
		fin.open(filename, ios::in);
		//处理头信息
		xml_quit(fin, origin);
		getline(fin, quit_be);
		T result;
		//根据容器总大小一一获得set对映类型数据
		for (int index = 0;index < total_num;index++)
		{
			to_xml::get_xml(result, fin, 0);
			//将获得的数据insert进入set类型
			set_type.insert(result);
		}
		getline(fin, quit_be);
		fin.close();
	}

	//vector类型转变xml文件
	//序列化函数
	template<typename T>
	int serialize_xml_vector(std::vector<T> vector_type, std::string name, std::string filename, int origin)
	{
		//填入头信息
		xml_header(name, filename, origin);
		fout.open(filename, ios::app);
		fout << "<vector>" << endl;
		fout.close();
		//同理，先获得容器的总大小
		int total_num = vector_type.size();
		//根据总大小一一写入对映类型的vector元素
		for (auto index = vector_type.begin();index != vector_type.end();index++)
			to_xml::serialize_xml(*index, name, filename, 0);
		
		fout.open(filename, ios::app);
		fout << "</vector>" << endl;
		fout.close();
		//填入尾信息
		xml_tail(name, filename, origin);
		return total_num;
	}
	//反序列函数
	template<typename T>
	void deserialize_xml_vector(std::vector<T>& vector_type, int total_num, std::string filename, int origin)
	{
		string quit_be;
		vector_type.clear();
		fin.open(filename, ios::in);
		//处理头信息
		xml_quit(fin, origin);
		getline(fin, quit_be);
		T result;
		for (int index = 0;index < total_num;index++)
		{
			//一一取出vector对映数据类型变量，并加入vector容器中
			to_xml::get_xml(result, fin, 0);
			vector_type.push_back(result);
		}
		getline(fin, quit_be);
		fin.close();
	}

	//list类型转变xml文件
	//序列化函数
	template<typename T>
	int serialize_xml_list(std::list<T> list_type, std::string name, std::string filename, int origin)
	{
		int number = 0;
		//填入头信息
		xml_header(name, filename, origin);
		fout.open(filename, ios::app);
		fout << "<list>" << endl;
		//获得容器大小并格式化写入文件
		int total_num = list_type.size();
		//根据容器大小，一一写入对映list数据类型数据
		fout.close();

		for (auto index = list_type.begin();index != list_type.end();index++)
			to_xml::serialize_xml(*index, name, filename, 0);

		//填入尾信息
		fout.open(filename, ios::app);
		fout << "</list>" << endl;
		fout.close();
		xml_tail(name, filename, origin);
		return total_num;
	}
	//反序列函数
	template<typename T>
	void deserialize_xml_list(std::list<T>& list_type, int total_num, std::string filename, int origin)
	{
		list_type.clear();
		string quit_be;
		fin.open(filename, ios::in);
		//处理头信息
		xml_quit(fin, origin);
		getline(fin, quit_be);
		T result;
		for (int index = 0;index < total_num;index++)
		{
			//一一读出对映类型元素并写入list中
			to_xml::get_xml(result, fin, 0);
			list_type.push_back(result);
		}
		getline(fin, quit_be);
		fin.close();
	}
}
