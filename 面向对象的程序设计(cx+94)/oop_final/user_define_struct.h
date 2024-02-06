#pragma once
#include<vector>
//自定义class样例
//设置serialize与deserialize的特殊之处会特别说明
struct UserDefinedType
{
	//自定义class对映的元素对象
	int idx;
	std::string name;
	std::vector<double> data;
	//定义一个默认构造函数，用于区别是否对一类对象赋值成功
	UserDefinedType() :idx(0), name("null") {
		data.push_back(1.0);
		data.push_back(2.0);
	};
	//二进制序列化与反序列化函数
	int serialize_bi(std::string filename);
	int deserialize_bi(std::string filename);
	//xml文件序列化与反序列化函数
	void serialize_myxml(std::string name, std::string filename, std::vector<int>& total_number);
	void deserialize_myxml(std::string name, std::string filename, std::vector<int>& total_number);
};
