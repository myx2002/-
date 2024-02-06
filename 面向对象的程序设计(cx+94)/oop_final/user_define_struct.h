#pragma once
#include<vector>
//�Զ���class����
//����serialize��deserialize������֮�����ر�˵��
struct UserDefinedType
{
	//�Զ���class��ӳ��Ԫ�ض���
	int idx;
	std::string name;
	std::vector<double> data;
	//����һ��Ĭ�Ϲ��캯�������������Ƿ��һ�����ֵ�ɹ�
	UserDefinedType() :idx(0), name("null") {
		data.push_back(1.0);
		data.push_back(2.0);
	};
	//���������л��뷴���л�����
	int serialize_bi(std::string filename);
	int deserialize_bi(std::string filename);
	//xml�ļ����л��뷴���л�����
	void serialize_myxml(std::string name, std::string filename, std::vector<int>& total_number);
	void deserialize_myxml(std::string name, std::string filename, std::vector<int>& total_number);
};
