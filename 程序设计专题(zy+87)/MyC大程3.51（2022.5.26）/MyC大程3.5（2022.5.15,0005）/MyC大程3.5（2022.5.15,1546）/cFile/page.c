//======================================================
//	page.c
//
//	��Ȩ�����ߣ��ʼ������
// 				�ƺ������Х�������� 
//
//	���һ���޸ģ�2022/5/5
//
//======================================================
#include "page.h"
#include"linkedlist.h"
#include"stdio.h"
#include"strlib.h"
#include "graphics.h"
#include "extgraph.h"
#include "string.h"

extern char fileName[100];									//�ļ���ַ����ַ�� 
extern page PageList[10];									//���ͬʱ����10����ҳ 
extern int ithWebPage; 										//��ǰ��ҳ
extern int webPageNum;									    //��ǰ����ҳ�� 

/*
 *	������: loadingPage
 *
 *	���ܣ������ļ�����ȡ�ļ�����  
 *
 *	�����β�: page�ṹָ�� nowpage, �ļ���(��ַ��pageLocation ����ֵ: �� 
 */
void loadingPage(page *nowPage, string pageLocation)
{
	if(strcmp(nowPage->location,pageLocation))				//������صĲ��ǵ�ǰҳ�� 
	{
		if(nowPage->location[0]!=0)							//���ԭ�Ȳ��ǿյ�ַ�������"����"���ܵ�ʵ�֣� 
		{
			strcpy(nowPage->formalPage, nowPage->location);	//��ԭ��ҳ���ַ��ֵ�� "��һҳ�� " 
		}		
	}

	FILE* fp;
	fp = fopen(pageLocation, "r+");				//�򿪶�Ӧ��ʵ���ļ� 
	strcpy(fileName, pageLocation);				//�����ص��ļ���ַ����ַ������fileName 
	strcpy(nowPage->location, pageLocation);		//�����ص��ļ���ַ����ַ��������ǰҳ���location 
	//�����ʧ��
	if(fp==NULL)
	{
		nowPage->location[0] = '\0';
		return;
	}
	//��ȡ�ļ���һ��һ���ڵ㴴������ 
	char buffer[200];					
	FreeLinkedList(nowPage->text);				//�ͷ�ԭ��ҳ����ı��������� 
	nowPage->text = NULL;
	nowPage->text = NewLinkedList();				//������ҳ����ı��������� 
	//��ÿһ���ı�����һ������ڵ� 
	while(!feof(fp))
	{
		fgets(buffer,200,fp);
		char *tmp = (char *)malloc(sizeof(char)*(strlen(buffer) + 1));	//+1��Ϊ�˸�'\0'����λ�� 
		strcpy(tmp, buffer);
		InsertNode(nowPage->text, NULL, tmp);
	}
	fclose(fp);
	//��ʼ��ҳ������ 
	nowPage->outputPageNum = 0;
	nowPage->textPageNum = 0;
	nowPage->ithOutputPage = 0;
	nowPage->ithTextPage = 0;
	int i;
	for(i = 0; i < 20; i++)
	{
		nowPage->textPageContent[i] = 0;
		nowPage->outputPageContent[i] = 0;		
	}
	nowPage->textPageContent[nowPage->ithTextPage] = nowPage->text;
	nowPage->outputPageContent[nowPage->ithOutputPage] = nowPage->text;
	nowPage->webPageName[0] = 0;
}

/*
 *	������: writrInPageName
 *
 *	���ܣ�д����ҳ���� 
 *
 *	�����β�: page�ṹָ�� nowpage	�ַ���a	����ֵ: �� 
 */
void writeInPageName(page *nowPage,string a)
{
	strcpy(nowPage->webPageName, a);
}

/*
 *	������: cancelPage
 *
 *	���ܣ��ر���ҳ 
 *
 *	�����β�: �رյ���ҳλ�� now	����ֵ: �� 
 */
void cancelPage(int canceledWebPage)
{

	//��ɾ���������һ����ҳ 
 	if(canceledWebPage==webPageNum)
 	{
  		memset(PageList + webPageNum, 0, sizeof(page));			//�����һ����ҳ���� 
		webPageNum--;
		ithWebPage = webPageNum;
  		return;
 	}
 	//��ɾ�������м����ҳ 
 	int i;
	for(i = canceledWebPage; i < webPageNum; i++)
 	{
  		memcpy(PageList + i, PageList + i + 1, sizeof(page));	//���ν���һ����ҳ�����ݸ��Ƹ���һ����ҳ 
 	}
 	memset(PageList + webPageNum, 0, sizeof(page));				//��ԭ�����һ����ҳ���� 
  	webPageNum--;

}
