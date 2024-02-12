//======================================================
//	analysis.c
//
//	��Ȩ�����ߣ��ʼ������
// 				�ƺ������Х�������� 
//
//	���һ���޸ģ�2022/5/15
//
//======================================================
#include "analysis.h"
#include "string.h"
#include "strlib.h"
#include "displaybox.h"
#include "graphics.h"
#include "extgraph.h"
#include "linkedlist.h" 

static int recordTag;		//��ǩ��¼ģʽ��־ 
static int recordText;		//�ı���¼ģʽ��־ 

/*
 *	������: apeendString
 *
 *	���ܣ������ַ��ӵ��ַ��� 
 *
 *	�����β�: char*, �����ַ�char	����ֵ: �� 
 */
void appendString(string a, char i)
{
	int p = strlen(a);
	a[p] = i;
	a[p + 1] = '\0';
}

/*
 *	������: dealInput
 *
 *	���ܣ��������ԭ�ı��ַ���
 *
 *	�����β�: �ַ��� s	����ֵ:	���������ʱ���� 1 ���� 0 
 */
int dealInput(char *s)
{
	char nowtag[200] = {};							//�洢��ǩ 
	int i;
	for(i = 0; i < strlen(s); i++)
	{
		if(s[i]=='<')
		{
			if(recordText)							//��֮ǰ�����ı���¼ģʽʱ 
			{
				if(dealText(nowtag))				//�����ı����ݣ��ҵ����������ʱ����1 
				{
					return 1; 
				}
			}
			recordText = 0;							//�ı���¼ģʽ���� 
			recordTag = 1;							//��ǩ��¼ģʽ��ʼ 
			nowtag[0] = 0;							//ͨ�����õ�һ��Ԫ��Ϊ0,ʹ��appendString�ڵ���strlenʱ��ͷ��ʼ��ֵ
		}
		else if(s[i]=='>')
		{	
			recordText = openTextInputMode(nowtag);	//��<>����Ǳ�ǩ���еı�ǩʱ���ı���¼ģʽ��������Ϊ 1
			dealTag(nowtag);   						//������ǩ 
			recordTag = 0;							//��ǩ��¼ģʽ��������Ϊ0 
			nowtag[0] = 0;							//ͨ�����õ�һ��Ԫ��Ϊ0,ʹ��appendString�ڵ���strlenʱ��ͷ��ʼ��ֵ 
		}
		else
		{
			if(s[i]!='\n')
			appendString(nowtag, s[i]);				//��¼<>�ڻ�������� 
		}
	}
	return 0;
}
