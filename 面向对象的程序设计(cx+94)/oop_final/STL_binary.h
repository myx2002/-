#pragma once
#include"type_binary.h"
#include<list>
#include<vector>
#include<map>
#include<set>
//����������ӳ�Ķ��������л���ת���л�
// ͳһ�����ʽ˵��
// 1.������������
// 2.�ļ���
// 3.����ļ�ͷƫ����
namespace to_binary_STL
{
	//����pair����
	//���л�
	template<typename T, typename I>
	int serialize_pair(std::pair<T, I> pair_type, std::string name, int offset)
	{
		//����ƫ��������
		int final_offset = offset;
		//��ӳ�ľ������ͣ�ֱ��ʹ��serialize_bi�������ɣ��������о�
		final_offset += to_binary::serialize_bi(pair_type.first, name, 0);
		final_offset += to_binary::serialize_bi(pair_type.second, name, 0);
		return final_offset;
	}
	//�����л�
	template<typename T, typename I>
	int deserialize_pair(std::pair<T, I>& pair_type, std::string name, int offset)
	{
		//����ƫ��������
		int final_offset = offset;
		//��ӳ�ľ������ͣ�ֱ��ʹ��deserialize_bi�������ɣ��������о�
		final_offset += to_binary::deserialize_bi(pair_type.first, name, final_offset);
		final_offset += to_binary::deserialize_bi(pair_type.second, name, final_offset);
		return final_offset;
	}
	//vector����ת��
	//���л�
	template<typename T>
	int serialize_vector(std::vector<T> vector_type, std::string name, int offset)
	{
		int final_offset = offset;
		//���������С������һһ����
		int total_vector = vector_type.size();
		//���ڸ�������д��
		final_offset += to_binary::serialize_bi(total_vector, name, 0);
		int index;
		for (index = 0;index < total_vector;index++)
			//��ӳ�ľ������ͣ�ֱ��ʹ��serialize_bi�������ɣ��������о�
			final_offset += to_binary::serialize_bi(vector_type[index], name, 0);
		return final_offset;
	}
	//�����л�
	template<typename T>
	int deserialize_vector(std::vector<T>& vector_type, std::string name, int offset)
	{
		vector_type.clear();
		int final_offset = offset;
		int total_vector;
		//���������С��ӳ������
		//��¼��ӳƫ����
		final_offset += to_binary::deserialize_bi(total_vector, name, final_offset);
		int index;
		T result;
		for (index = 0;index < total_vector;index++)
		{
			//һһ��ȡ��Ҫ������Ԫ��
			final_offset += to_binary::deserialize_bi(result, name, final_offset);
			//װ���ȡָ����ŵ�����
			vector_type.push_back(result);
		}
		return final_offset;
	}
	//list���͵�ת��
	//���л�
	template<typename T>
	int serialize_list(std::list<T> list_type, std::string name, int offset)
	{
		int final_offset = offset;
		//���������С��ӳ������
		int total_list = list_type.size();
		//��¼��ӳƫ����
		final_offset += to_binary::serialize_bi(total_list, name, 0);
		//������ӳ�ĵ�����
		for (auto index = list_type.begin();index != list_type.end();index++)
			//һһд��������ļ�
			final_offset += to_binary::serialize_bi(*index, name, 0);
		return final_offset;
	}
	//�����л�
	template<typename T>
	int deserialize_list(std::list<T>& list_type, std::string name, int offset)
	{
		list_type.clear();
		int final_offset = offset;
		int list_vector;
		//���������С��ӳ������
		//��¼��ӳƫ����
		final_offset += to_binary::deserialize_bi(list_vector, name, final_offset);
		int index;
		T result;
		for (index = 0;index < list_vector;index++)
		{
			//һһ�Ӷ������ļ��ж�����ӳ���͵�Ԫ��
			final_offset += to_binary::deserialize_bi(result, name, final_offset);
			//�����ӳ��������
			list_type.push_back(result);
		}
		return final_offset;
	}
	//set���͵�ת��
	//���л�
	template<typename T>
	int serialize_set(std::set<T> set_type, std::string name, int offset)
	{
		int final_offset = offset;
		//���������С
		int total_set = set_type.size();
		//���ڶ������ļ��м�¼�������ܸ���
		final_offset += to_binary::serialize_bi(total_set, name, 0);
		//������������һһд��������ļ�
		for (auto index = set_type.begin();index != set_type.end();index++)
			final_offset += to_binary::serialize_bi(*index, name, 0);
		return final_offset;
	}
	//�����л�
	template<typename T>
	int deserialize_set(std::set<T>& set_type, std::string name, int offset)
	{
		set_type.clear();
		int final_offset = offset;
		int set_vector;
		//��������ܴ�С������һһ����Ԫ��
		final_offset += to_binary::deserialize_bi(set_vector, name, final_offset);
		int index;
		T result;
		for (index = 0;index < set_vector;index++)
		{
			//һһ����Ԫ�ز�insert����set��
			final_offset += to_binary::deserialize_bi(result, name, final_offset);
			set_type.insert(result);
		}
		return final_offset;
	}
	//map���͵�ת��
	//���л�
	template<typename T, typename I>
	int serialize_map(std::map<T, I> map_type, std::string name, int offset)
	{
		int final_offset = offset;
		//���map�����ܴ�С
		int total_map = map_type.size();
		//��¼д��������ļ���������С����
		final_offset += to_binary::serialize_bi(total_map, name, 0);
		for (auto index = map_type.begin();index != map_type.end();index++)
		{
			//mapÿһ��Ԫ�ض�ӳ������������һһд��
			final_offset += to_binary::serialize_bi(index->first, name, 0);
			final_offset += to_binary::serialize_bi(index->second, name, 0);
		}
		return final_offset;
	}
	//�����л�
	template<typename T, typename I>
	int deserialize_map(std::map<T, I>& map_type, std::string name, int offset)
	{
		map_type.clear();
		int final_offset = offset;
		int map_vector;
		//��������ӵ�е����ݸ���
		final_offset += to_binary::deserialize_bi(map_vector, name, final_offset);
		int index;
		T result_one;
		I result_other;
		//����Ԫ���ܸ���һһ����map class������ֵ
		for (index = 0;index < map_vector;index++)
		{
			//����map��ÿһ��Ԫ�ض�ӳ��first��second����ֵ�������и�ֵ
			final_offset += to_binary::deserialize_bi(result_one, name, final_offset);
			final_offset += to_binary::deserialize_bi(result_other, name, final_offset);
			map_type[result_one] = result_other;
		}
		return final_offset;
	}
}