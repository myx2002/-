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
//�費��Ҫstd::����

//---**����ת��Ϊxml�ṹ**---
// ͳһ�����ʽ˵��
// 1.�����������
// 2.�ļ���ʶ��
// 3.�ļ���
// 4.�Ƿ�Ϊ��д����ã��û�ʹ��Ĭ��Ϊ1��
//pairת��Ϊxml
namespace to_xml_STL
{
	//pair����ת��xml�ļ�
	//���л�����
	template<typename T, typename I>
	int serialize_xml_pair(std::pair<T, I> pair_type, std::string name, std::string filename, int origin)
	{
		//����ͷ��Ϣ
		xml_header(name, filename, origin);

		fout.open(filename, ios::app);
		xml_format(fout, 0);
		fout << "<pair>" << endl;
		//��ʽ��д��pair��ӳ��first����������
		xml_format(fout, 0);
		fout << "<first>" << endl;
		fout.close();

		to_xml::serialize_xml(pair_type.first, name, filename, 0);

		fout.open(filename, ios::app);
		xml_format(fout, 0);
		fout << "</first>" << endl;
		//��ʽ��д��pair��ӳ��second����������
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

		//����β��Ϣ
		xml_tail(name, filename, origin);

		return 2;
	}
	//�����к���
	template<typename T, typename I>
	void deserialize_xml_pair(std::pair<T, I>& pair_type, int total_num, std::string filename, int origin)
	{
		string quit_be;
		fin.open(filename, ios::in);
		//����ͷ��Ϣ
		xml_quit(fin, origin);
		//����ͷ��Ϣ
		getline(fin, quit_be);
		//����ӳ���������Ͷ�ȡpair��ӳ��first/second��������
		to_xml::get_xml(pair_type.first, fin, 1);
		to_xml::get_xml(pair_type.second, fin, 1);
		getline(fin, quit_be);
		fin.close();
	}


	//map����ת��xml�ļ�
	//���л�����
	template<typename T, typename I>
	int serialize_xml_map(std::map<T, I> map_type, std::string name, std::string filename, int origin)
	{
		//����ͷ��Ϣ
		xml_header(name, filename, origin);
		
		int number = 0;
		//����ͷ��Ϣ
		fout.open(filename, ios::app);
		fout << "<map>" << endl;
		//�Ȼ��map������ӳ�Ĵ�С��д���ļ������ں������ж������һд��
		int total_num = map_type.size();
		//��ʽ��д��size����
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

		//����β��Ϣ
		xml_tail(name, filename, origin);

		return total_num;
	}
	//�����к���
	template<typename T, typename I>
	void deserialize_xml_map(std::map<T, I>& map_type, int total_num, std::string filename, int origin)
	{
		string quit_be;
		map_type.clear();
		fin.open(filename, ios::in);
		//����ͷ��Ϣ
		xml_quit(fin, origin);
		getline(fin, quit_be);
		//����ͷ��Ϣ
		T result_one;
		I result_two;
		//����������С�����map���ж����ӳ��first/second����
		for (int index = 0;index < total_num;index++)
		{	
			getline(fin, quit_be);
			to_xml::get_xml(result_one, fin, 1);
			to_xml::get_xml(result_two, fin, 1);
			//������map������Ҫ��first������second������һһ��Ӧ
			map_type[result_one] = result_two;
			getline(fin, quit_be);
		}
		getline(fin, quit_be);
		fin.close();
	}

	//set����ת��xml�ļ�
	//���л�����
	template<typename T>
	int serialize_xml_set(std::set<T> set_type, std::string name, std::string filename, int origin)
	{
		//����ͷ��Ϣ
		xml_header(name, filename, origin);

		fout.open(filename, ios::app);
		fout << "<set>" << endl;
		//�Ȼ��set������ӳ�Ĵ�С��д���ļ������ں������ж������һд��
		int total_num = set_type.size();
		//��������������һһд��setԪ��
		fout.close();
		for (auto index = set_type.begin();index != set_type.end();index++)
			to_xml::serialize_xml(*index, name, filename, 0);

		fout.open(filename, ios::app);
		fout << "</set>" << endl;
		fout.close();
		//����β��Ϣ
		xml_tail(name, filename, origin);
		return total_num;
	}
	//�����к���
	template<typename T>
	void deserialize_xml_set(std::set<T>& set_type, int total_num, std::string filename, int origin)
	{
		string quit_be;
		set_type.clear();
		fin.open(filename, ios::in);
		//����ͷ��Ϣ
		xml_quit(fin, origin);
		getline(fin, quit_be);
		T result;
		//���������ܴ�Сһһ���set��ӳ��������
		for (int index = 0;index < total_num;index++)
		{
			to_xml::get_xml(result, fin, 0);
			//����õ�����insert����set����
			set_type.insert(result);
		}
		getline(fin, quit_be);
		fin.close();
	}

	//vector����ת��xml�ļ�
	//���л�����
	template<typename T>
	int serialize_xml_vector(std::vector<T> vector_type, std::string name, std::string filename, int origin)
	{
		//����ͷ��Ϣ
		xml_header(name, filename, origin);
		fout.open(filename, ios::app);
		fout << "<vector>" << endl;
		fout.close();
		//ͬ���Ȼ���������ܴ�С
		int total_num = vector_type.size();
		//�����ܴ�Сһһд���ӳ���͵�vectorԪ��
		for (auto index = vector_type.begin();index != vector_type.end();index++)
			to_xml::serialize_xml(*index, name, filename, 0);
		
		fout.open(filename, ios::app);
		fout << "</vector>" << endl;
		fout.close();
		//����β��Ϣ
		xml_tail(name, filename, origin);
		return total_num;
	}
	//�����к���
	template<typename T>
	void deserialize_xml_vector(std::vector<T>& vector_type, int total_num, std::string filename, int origin)
	{
		string quit_be;
		vector_type.clear();
		fin.open(filename, ios::in);
		//����ͷ��Ϣ
		xml_quit(fin, origin);
		getline(fin, quit_be);
		T result;
		for (int index = 0;index < total_num;index++)
		{
			//һһȡ��vector��ӳ�������ͱ�����������vector������
			to_xml::get_xml(result, fin, 0);
			vector_type.push_back(result);
		}
		getline(fin, quit_be);
		fin.close();
	}

	//list����ת��xml�ļ�
	//���л�����
	template<typename T>
	int serialize_xml_list(std::list<T> list_type, std::string name, std::string filename, int origin)
	{
		int number = 0;
		//����ͷ��Ϣ
		xml_header(name, filename, origin);
		fout.open(filename, ios::app);
		fout << "<list>" << endl;
		//���������С����ʽ��д���ļ�
		int total_num = list_type.size();
		//����������С��һһд���ӳlist������������
		fout.close();

		for (auto index = list_type.begin();index != list_type.end();index++)
			to_xml::serialize_xml(*index, name, filename, 0);

		//����β��Ϣ
		fout.open(filename, ios::app);
		fout << "</list>" << endl;
		fout.close();
		xml_tail(name, filename, origin);
		return total_num;
	}
	//�����к���
	template<typename T>
	void deserialize_xml_list(std::list<T>& list_type, int total_num, std::string filename, int origin)
	{
		list_type.clear();
		string quit_be;
		fin.open(filename, ios::in);
		//����ͷ��Ϣ
		xml_quit(fin, origin);
		getline(fin, quit_be);
		T result;
		for (int index = 0;index < total_num;index++)
		{
			//һһ������ӳ����Ԫ�ز�д��list��
			to_xml::get_xml(result, fin, 0);
			list_type.push_back(result);
		}
		getline(fin, quit_be);
		fin.close();
	}
}
