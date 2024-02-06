#pragma once
#include"type_binary.h"
#include<list>
#include<vector>
#include<map>
#include<set>
//定义容器对映的二进制序列化与转序列化
// 统一输出格式说明
// 1.输入容器类型
// 2.文件名
// 3.相对文件头偏移量
namespace to_binary_STL
{
	//对于pair容器
	//序列化
	template<typename T, typename I>
	int serialize_pair(std::pair<T, I> pair_type, std::string name, int offset)
	{
		//定义偏移量即可
		int final_offset = offset;
		//对映的具体类型，直接使用serialize_bi函数即可，重载自判决
		final_offset += to_binary::serialize_bi(pair_type.first, name, 0);
		final_offset += to_binary::serialize_bi(pair_type.second, name, 0);
		return final_offset;
	}
	//反序列化
	template<typename T, typename I>
	int deserialize_pair(std::pair<T, I>& pair_type, std::string name, int offset)
	{
		//定义偏移量即可
		int final_offset = offset;
		//对映的具体类型，直接使用deserialize_bi函数即可，重载自判决
		final_offset += to_binary::deserialize_bi(pair_type.first, name, final_offset);
		final_offset += to_binary::deserialize_bi(pair_type.second, name, final_offset);
		return final_offset;
	}
	//vector类型转变
	//序列化
	template<typename T>
	int serialize_vector(std::vector<T> vector_type, std::string name, int offset)
	{
		int final_offset = offset;
		//获得容器大小，方便一一存入
		int total_vector = vector_type.size();
		//对于个数整型写入
		final_offset += to_binary::serialize_bi(total_vector, name, 0);
		int index;
		for (index = 0;index < total_vector;index++)
			//对映的具体类型，直接使用serialize_bi函数即可，重载自判决
			final_offset += to_binary::serialize_bi(vector_type[index], name, 0);
		return final_offset;
	}
	//反序列化
	template<typename T>
	int deserialize_vector(std::vector<T>& vector_type, std::string name, int offset)
	{
		vector_type.clear();
		int final_offset = offset;
		int total_vector;
		//获得容器大小对映的整数
		//记录对映偏移量
		final_offset += to_binary::deserialize_bi(total_vector, name, final_offset);
		int index;
		T result;
		for (index = 0;index < total_vector;index++)
		{
			//一一读取所要的容器元素
			final_offset += to_binary::deserialize_bi(result, name, final_offset);
			//装入读取指定存放的容器
			vector_type.push_back(result);
		}
		return final_offset;
	}
	//list类型的转变
	//序列化
	template<typename T>
	int serialize_list(std::list<T> list_type, std::string name, int offset)
	{
		int final_offset = offset;
		//获得容器大小对映的整数
		int total_list = list_type.size();
		//记录对映偏移量
		final_offset += to_binary::serialize_bi(total_list, name, 0);
		//遍历对映的迭代器
		for (auto index = list_type.begin();index != list_type.end();index++)
			//一一写入二进制文件
			final_offset += to_binary::serialize_bi(*index, name, 0);
		return final_offset;
	}
	//反序列化
	template<typename T>
	int deserialize_list(std::list<T>& list_type, std::string name, int offset)
	{
		list_type.clear();
		int final_offset = offset;
		int list_vector;
		//获得容器大小对映的整数
		//记录对映偏移量
		final_offset += to_binary::deserialize_bi(list_vector, name, final_offset);
		int index;
		T result;
		for (index = 0;index < list_vector;index++)
		{
			//一一从二进制文件中读出对映类型的元素
			final_offset += to_binary::deserialize_bi(result, name, final_offset);
			//放入对映的容器中
			list_type.push_back(result);
		}
		return final_offset;
	}
	//set类型的转变
	//序列化
	template<typename T>
	int serialize_set(std::set<T> set_type, std::string name, int offset)
	{
		int final_offset = offset;
		//获得容器大小
		int total_set = set_type.size();
		//先在二进制文件中记录容器中总个数
		final_offset += to_binary::serialize_bi(total_set, name, 0);
		//迭代器遍历并一一写入二进制文件
		for (auto index = set_type.begin();index != set_type.end();index++)
			final_offset += to_binary::serialize_bi(*index, name, 0);
		return final_offset;
	}
	//反序列化
	template<typename T>
	int deserialize_set(std::set<T>& set_type, std::string name, int offset)
	{
		set_type.clear();
		int final_offset = offset;
		int set_vector;
		//获得容器总大小，方便一一读出元素
		final_offset += to_binary::deserialize_bi(set_vector, name, final_offset);
		int index;
		T result;
		for (index = 0;index < set_vector;index++)
		{
			//一一读出元素并insert进入set中
			final_offset += to_binary::deserialize_bi(result, name, final_offset);
			set_type.insert(result);
		}
		return final_offset;
	}
	//map类型的转变
	//序列化
	template<typename T, typename I>
	int serialize_map(std::map<T, I> map_type, std::string name, int offset)
	{
		int final_offset = offset;
		//获得map容器总大小
		int total_map = map_type.size();
		//记录写入二进制文件的容器大小整数
		final_offset += to_binary::serialize_bi(total_map, name, 0);
		for (auto index = map_type.begin();index != map_type.end();index++)
		{
			//map每一个元素对映于两个变量，一一写入
			final_offset += to_binary::serialize_bi(index->first, name, 0);
			final_offset += to_binary::serialize_bi(index->second, name, 0);
		}
		return final_offset;
	}
	//反序列化
	template<typename T, typename I>
	int deserialize_map(std::map<T, I>& map_type, std::string name, int offset)
	{
		map_type.clear();
		int final_offset = offset;
		int map_vector;
		//读出容器拥有的数据个数
		final_offset += to_binary::deserialize_bi(map_vector, name, final_offset);
		int index;
		T result_one;
		I result_other;
		//根据元素总个数一一读出map class的属性值
		for (index = 0;index < map_vector;index++)
		{
			//读出map中每一个元素对映的first与second属性值，并进行赋值
			final_offset += to_binary::deserialize_bi(result_one, name, final_offset);
			final_offset += to_binary::deserialize_bi(result_other, name, final_offset);
			map_type[result_one] = result_other;
		}
		return final_offset;
	}
}