#pragma once
#include<iostream>
#include<string>
#include<vector>
using namespace std;

//�����ռ�����
class diary {
private:
	//���ڱ���
	string date;
	//ʵ�����
	string entity;

public:
	//�ı�����������ݺ���
	void set(string da, string en);
	//�������
	void print();
	//�����������ݺ���
	void swap(diary* num);
	//������ڱ�������
	string viewdate();
	//������ݺ���
	string viewentity();
	//�ȶ����ں���
	int pdcheck(string da);
};

//ɾ��������
void pdremove(vector<diary>& newarray);
//ָ������չʾ����
void pdshow(vector<diary>& newarray);
//�б�չʾ����
void pdlist(vector<diary>& newarray);
//�����������
void pdadd(vector<diary>& newarray);

