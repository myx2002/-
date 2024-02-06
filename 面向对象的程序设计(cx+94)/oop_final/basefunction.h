#pragma once
//---**ͨ�ú���**---
//�����Ƽ���
void sub_int(std::vector<int>& int_array, int index);
//������ȡ��
void reverse(std::vector<int>& int_array);
//������ļ�
void print_to_file(std::vector<int>& int_array, std::ofstream& fout);
//���ļ�����
void read_from_file(std::vector<int>& int_array, int total, std::ifstream& fin);
//��������ת��Ϊ����
int binary_to_int(std::vector<int>& int_array, int total);
//���ڳ�����
long long int binary_to_llint(std::vector<int>& int_array, int total);
//����ת��Ϊ��������
//ģ��ֱ�Ӷ�����.h�ļ�
template<typename T>
void int_to_binary(std::vector<int>& int_array, T number, int total)
{
	int index;
	//����λ��������һȡ����λ��������������
	for (index = total - 1;index >= 0;index--)
		int_array.push_back((number >> index) & 1);
}