//======================================================
//	enumType.h
//
//	��Ȩ�����ߣ��ʼ������
// 				�ƺ������Х�������� 
//
//	���һ���޸ģ�2022/5/5
//
//======================================================

#ifndef __ENUMTYPE_H__
#define __ENUMTYPE_H__

//ö���ı�������ͣ���ͨ���»��ߡ�ɾ���ߡ���д�������ӡ����� 
enum printType
{
	normalText, insText, delText, abbrText, hrefText, titleText
};

//ö�����������ͣ�ʵ�ߡ����� 
enum lineTypeEnum
{
	normalLine, dotLine
};

//ö��������λ�ã��ϡ��С��� 
enum lineLocationEnum
{
	down, middle, up
};

#endif 
