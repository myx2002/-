#pragma once
extern std::ifstream fin;
extern std::ofstream fout;

//---**һ����������ת��Ϊxml�ṹ**---
// ͳһ�����ʽ˵����serialize_xml��deserialize_xml������
// 1.�����������
// 2.�ļ���ʶ��
// 3.�ļ���
// 4.�Ƿ�Ϊ��д����ã��û�ʹ��Ĭ��Ϊ1��

//get_xml����
//1.��ӳ�������ͽ����Ų���
//2.�ļ�������
//3.�Ƿ�Ϊ��������ã��û�ʹ��Ĭ��Ϊ1��
namespace to_xml
{
	//intת��Ϊxml
	//���л����⺯�������ڱ������Զ��������
	void get_xml(int& result, std::ifstream& fin, int origin);
	//���л�����
	void serialize_xml(int int_type, std::string name, std::string filename, int origin);
	//�����л�����
	void deserialize_xml(int& int_type, std::string name, std::string filename, int origin);
	//boolת��Ϊxml
	//���л����⺯�������ڱ������Զ��������
	void get_xml(bool& result, std::ifstream& fin, int origin);
	//���л�����
	void serialize_xml(bool bool_type, std::string name, std::string filename, int origin);
	//�����л�����
	void deserialize_xml(bool& bool_type, std::string name, std::string filename, int origin);
	//charת��Ϊxml
	//���л����⺯�������ڱ������Զ��������
	void get_xml(char& result, std::ifstream& fin, int origin);
	//���л�����
	void serialize_xml(char char_type, std::string name, std::string filename, int origin);
	//�����л�����
	void deserialize_xml(char& char_type, std::string name, std::string filename, int origin);
	//floatת��Ϊxml
	//���л����⺯�������ڱ������Զ��������
	void get_xml(float& result, std::ifstream& fin, int origin);
	//���л�����
	void serialize_xml(float float_type, std::string name, std::string filename, int origin);
	//�����л�����
	void deserialize_xml(float& float_type, std::string name, std::string filename, int origin);
	//doubleת��Ϊxml
	//���л����⺯�������ڱ������Զ��������
	void get_xml(double& result, std::ifstream& fin, int origin);
	//���л�����
	void serialize_xml(double double_type, std::string name, std::string filename, int origin);
	//�����л�����
	void deserialize_xml(double& double_type, std::string name, std::string filename, int origin);
	//stringת��Ϊxml
	//���л����⺯�������ڱ������Զ��������
	void get_xml(std::string& result, std::ifstream& fin, int origin);
	//���л�����
	void serialize_xml(std::string string_type, std::string name, std::string filename, int origin);
	//�����л�����
	void deserialize_xml(std::string& string_type, std::string name, std::string filename, int origin);
}
