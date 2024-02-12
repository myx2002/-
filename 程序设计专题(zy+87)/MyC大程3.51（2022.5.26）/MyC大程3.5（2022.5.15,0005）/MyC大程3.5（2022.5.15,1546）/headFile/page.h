//======================================================
//	page.h
//
//	��Ȩ�����ߣ��ʼ������
// 				�ƺ������Х�������� 
//
//	���һ���޸ģ�2022/5/15
//
//======================================================

#ifndef __PAGE_H__
#define __PAGE_H__

#include"linkedlist.h"
#include"strlib.h"

//�Զ���һ��page�ṹ�����ڼ�¼�ļ�������ַ������ҳ�����ļ������ݡ�ǰ��ҳ�� 
typedef struct page{
	char location[30];						//�ļ�������ַ�� 
	char webPageName[30];					//��ҳ�� 
	linkedlistADT text;						//�洢һ���ļ��������� 
	linkedlistADT textPageContent[20];		//�洢�ļ���ÿһҳ�������� 
	linkedlistADT outputPageContent[20];	//�洢�ļ���ÿһҳ������� 
	char formalPage[30];					//��һҳ�ļ� 
	char nextPage[30];						//��һҳ�ļ� 
	int textPageNum;						//HTML�ı���ҳ��  
	int outputPageNum;						//����������ҳ�� 
	int ithTextPage;						//��ǰHTML�ı�ҳ�� 
	int ithOutputPage;						//��ǰ��������ҳ�� 
}page;

/*
 *	������: loadingPage
 *
 *	���ܣ������ļ�����ȡ�ļ����� 
 *
 *	�����β�: page�ṹָ�� nowpage, �ļ���(��ַ��pageLocation ����ֵ: �� 
 */
void loadingPage(page *nowPage, string pageLocation);

/*
 *	������: writrInPageName
 *
 *	���ܣ�д����ҳ���� 
 *
 *	�����β�: page�ṹָ�� nowpage	�ַ���a	����ֵ: �� 
 */
void writeInPageName(page *nowPage, string a);

/*
 *	������: cancelPage
 *
 *	���ܣ��ر���ҳ 
 *
 *	�����β�: �رյ���ҳλ�� now	����ֵ: �� 
 */
void cancelPage(int ithWebPage);

#endif 
