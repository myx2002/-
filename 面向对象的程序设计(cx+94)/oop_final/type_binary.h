#pragma once
//---**��ͨ���Ͷ�����ת�䲿��**---
// ͳһ�����ʽ˵��
// 1.������������
// 2.�ļ���
// 3.������ļ�ͷƫ����
namespace to_binary
{
	//intת��
	//���л�����
	int serialize_bi(int int_type, std::string name, int offset);
	//�����л�����
	int deserialize_bi(int& int_rece, std::string name, int offset);

	//floatת��
	//���л�����
	int serialize_bi(float float_type, std::string name, int offset);
	//�����л�����
	int deserialize_bi(float& float_rece, std::string name, int offset);

	//charת��
	//���л�����
	int serialize_bi(char char_type, std::string name, int offset);
	//�����л�����
	int deserialize_bi(char& char_rece, std::string name, int offset);

	//bool����ת��
	//���л�����
	int serialize_bi(bool bool_type, std::string name, int offset);
	//�����л�����
	int deserialize_bi(bool& bool_rece, std::string name, int offset);

	//double����ת��
	//���л�����
	int serialize_bi(double double_type, std::string name, int offset);
	//�����л�����
	int deserialize_bi(double& double_rece, std::string name, int offset);

	//string����ת��
	//���л�����
	int serialize_bi(std::string string_type, std::string name, int offset);
	//�����л�����
	int deserialize_bi(std::string& string_rece, std::string name, int offset);
}
extern std::ifstream fin;
extern std::ofstream fout;
	