//======================================================
//	analysisTag.c
//
//	��Ȩ�����ߣ��ʼ������
// 				�ƺ������Х�������� 
//
//	���һ���޸ģ�2022/5/15
//
//======================================================
#include "analysisTag.h"
#include "string.h"
#include "strlib.h"
#include "linkedlist.h"
#include "displaybox.h"
#include "graphics.h"
#include "extgraph.h" 
#include "enumType.h" 

char buffer[100];				//�洢�ı��ؼ���Ӧ���� 
extern displayBox outputBox;
//��ǩ��(������shiftTag) 
char *TextInputOpenTag[] = {"p", "title", "h1", "h2", "h3", "h4", "h5", "h6",
						  "b", "/b", "i", "/i", "sub", "/sub", "sup", "/sup",
						  "br", "strong", "/strong", "em", "/em", "blockquote",
						  "/blockquote", "q", "/q", "cite", "/cite", "dfn", "/dfn",
						  "abbr", "/abbr", "ins", "/ins", "del", "/del", "a", "/a", "li"};
//��Ҫ���еĽ�����ǩ 
char *shiftTag[] = {"/p", "/h1", "/h2", "/h3", "/h4", "/h5", "/h6", "/blockquote", "/li"};

enum printType textType = normalText;		 
						  
//�洢��ǩ���ԺͶ�Ӧ���ݵĽṹ 
typedef struct attribute{
	string attributeName;
	string attributeValue;
}attribute;

int list = 0;			//�б����� 
int listNum[10];		//�������¼�����б���б�������ã��������ţ� 

/*
 *	������: dealTag
 *
 *	���ܣ���һ����ȡ��ǩ����ִ����Ӧ���� 
 *
 *	�����β�: ������ǩ fulltag	����ֵ: �� 
 */
void dealTag(string fullTag)
{
	if(fullTag[0]=='!')									//�ñ�ǩΪע��
	{
		return;
	}
	string tag;
	linkedlistADT attributeLink=NULL;
	if(FindChar(' ', fullTag, 0)!=-1)					//����ǩ���пո�ʱ 
	{
		tag = fullTag;									//�ѿո�ǰ�沿�ָ���tag 
		fullTag+=FindChar(' ', fullTag, 0);				//�ѿո�ʼ���沿�ָ���fullTag 
		fullTag[0] = 0;									//�ѿո��Ϊ'\0'�ָ�,��ʱfullTag[0]='\0' 
		fullTag++;							
		if(FindChar('=', fullTag, 0)!=-1)				//��fullTag����'='ʱ 
		{
			attributeLink = NewLinkedList();
			while(FindChar('=', fullTag, 0)!=-1)		//��ǩ�п����в�ֹһ������ 
			{
				attribute* nowAttribute = (attribute*)malloc(sizeof(attribute));
				nowAttribute->attributeName = fullTag;	//�ѵȺ�ǰ�沿�ָ���atrributeName 
				fullTag+=FindChar('=', fullTag, 0);
				fullTag[0] = 0;
				fullTag[1] = 0;							//��'='��'"'��Ϊ0  
				fullTag+=2;	
				nowAttribute->attributeValue = fullTag;	//�����ź��沿�ָ���attributeValue 
				ConvertToLowerCase(nowAttribute->attributeName);
				fullTag+=FindChar('"', fullTag, 0);
				fullTag[0] = 0;
				fullTag[1] = 0;
				fullTag+=2;
				InsertNode(attributeLink, NULL, nowAttribute);
			}
		}
	}
	else
	{
		tag = fullTag;
	}
	ConvertToLowerCase(tag);							//��Ϊ�Դ�С�����У�ͳһת����Сд�ж� 
	int num = sizeof(shiftTag)/sizeof(char*);	
	int i;
	for(i = 0; i < num; i++)
	{
		if(strcmp(tag,shiftTag[i])==0)
		{
			shift(&outputBox);							//���tag����Ҫ���еĽ�����ǩ�ͻ��� 
		}
	}
	//�Խ�����ǩ����Ӧ���� 
	if(tag[0]=='/')	
	{
		SetStyle(0);
		outputBox.fontSize = outputBox.globalFontSize;	//�ָ�Ĭ��ֵ 
		if(!strcmp(tag, "/sup"))						//�ϱ���� 
		{
			SetPointSize(outputBox.fontSize);
			outputBox.nowPrintY-=GetFontHeight() * 0.4;
		}
		if(!strcmp(tag, "/blockquote"))					//������ý��� 
		{
			outputBox.boxSideGap-=outputBox.boxWdith / 8;
			resetBoxSideGap(&outputBox);
		}
		if(!strcmp(tag, "/q"))							//�����ý��� 
		{
			writeInBox(CharToString('"'), &outputBox);			
		}
		if(!strcmp(tag, "/ul"))							//�����б� 
		{
			list--;
			printf("%d\n", list);
		}
		if(!strcmp(tag, "/ol"))							//�����б� 
		{
			listNum[list] = 0;
			list--;
		}
	}
	//�������ǩ����Ӧ���� 
	if(!strcmp(tag, "h1"))			//һ������ 
	{
		outputBox.fontSize = 50;
		SetStyle(1);
	}
	else if(!strcmp(tag, "h2"))		//�������� 
	{
		outputBox.fontSize = 45;
		SetStyle(1);
	}
	else if(!strcmp(tag, "h3"))		//�������� 
	{
		outputBox.fontSize = 40;
		SetStyle(1);
	}
	else if(!strcmp(tag, "h4"))		//�ļ����� 
	{
		outputBox.fontSize = 35;
		SetStyle(1);
	}
	else if(!strcmp(tag, "h5"))		//�弶���� 
	{
		outputBox.fontSize = 30;
		SetStyle(1);
	}
	else if(!strcmp(tag, "h6"))		//�������� 
	{
		outputBox.fontSize = 25;
		SetStyle(1);
	}
	else if(!strcmp(tag, "p"))		//���� 
	{
		outputBox.fontSize = 20;
		SetStyle(0);
	}
	else if(!strcmp(tag, "b") || !strcmp(tag, "strong"))//���� 
	{
		SetStyle(1);
	}
	else if(!strcmp(tag, "i") || !strcmp(tag, "em"))	//б�� 
	{
		SetStyle(2);
	}
	else if(!strcmp(tag, "sub") || !strcmp(tag, "sup"))	//�±� or�ϱ� 
	{
		int tmpFontSize = outputBox.fontSize;
		double nowHeight = GetFontHeight();
		do
		{
			tmpFontSize--;
			SetPointSize(tmpFontSize);
		}while(GetFontHeight() > nowHeight * 0.5);
		outputBox.fontSize = tmpFontSize;
		if(!strcmp(tag, "sup"))
		{
			outputBox.nowPrintY+=nowHeight * 0.4;
		}
	}
	else if(!strcmp(tag, "br"))							//���� 
	{
		shift(&outputBox);
	}
	else if(!strcmp(tag, "hr"))							//�ָ��� 
	{
		drawHorizentalLine(&outputBox);
		shift(&outputBox); 
	}
	else if(!strcmp(tag, "blockquote")) 				//������� 
	{
		outputBox.boxSideGap+=outputBox.boxWdith / 8;	//����ҳ��߾� 
		resetBoxSideGap(&outputBox);					//����ҳ��߾������������λ�� 
	}
	else if(!strcmp(tag, "q"))							//������ 
	{
		writeInBox(CharToString('"'), &outputBox);
	}
	else if(!strcmp(tag, "ins"))						//�»��� 
	{
		textType = insText;								//���ı���������Ϊ�»�������
	}
	else if(!strcmp(tag, "del"))						//ɾ���� 
	{
		textType = delText;								//���ı���������Ϊɾ��������
	}
	else if(!strcmp(tag, "title"))						//��ҳ���� 
	{
		textType = titleText;							//���ı���������Ϊ��������
	}
	else if(!strcmp(tag, "ul"))							//�����б� 
	{
		list++;
	}
	else if(!strcmp(tag, "ol"))							//�����б� 
	{
		list++;
		listNum[list]++;
	}
	else if(!strcmp(tag, "li"))							//�����б��� 
	{
		//ÿǶ��һ���б�Ͷ��һ�οո�� 
		int i;
		for(i = 1; i<=list; i++)
		{
			writeInBox("  ", &outputBox);				
		}
		//���������б�ʱ 
		if(!listNum[list])
		{
			drawCircle(&outputBox, list);
		}
		//���������б�ʱ 
		else
		{
			writeInBox(IntegerToString(listNum[list]), &outputBox);	//������ 
			writeInBox(".", &outputBox);	
			listNum[list]++;										//���+1 
		}
	}
	else if(!strcmp(tag, "abbr"))			//��д 
	{
		textType = abbrText;				//���ı���������Ϊ��д���� 
		if(attributeLink!=NULL)
		{
			linkedlistADT tmp = attributeLink->next;
			while(tmp!=NULL)
			{
				//�жϲ�ͬ������ 
				if(!strcmp("title", ((attribute*)(tmp->dataptr))->attributeName))	//�ж� attributeName�Ƿ�Ϊ title 
				{
					strcpy(buffer, ((attribute*)(tmp->dataptr))->attributeValue);	//�� attrbuteValue����buffer 
				}
				tmp = tmp->next;
			}
		}
	}
	else if(!strcmp(tag, "a"))			//������ 
	{
		textType = hrefText;			//���ı���������Ϊ����������		
		if(attributeLink!=NULL)
		{
			linkedlistADT tmp = attributeLink->next;
			while(tmp!=NULL)
			{
				if(!strcmp("href", ((attribute*)(tmp->dataptr))->attributeName))	//�ж� attributeName�Ƿ�Ϊ href 
				{
					strcpy(buffer, ((attribute*)(tmp->dataptr))->attributeValue);	//�� attrbuteValue����buffer 
				}
				tmp = tmp->next;
			}
		}
	}
}

/*
 *	������: openTextInputMode
 *
 *	���ܣ��ж��Ƿ��ǿ��к��еı�ǩ�Ծ����Ƿ��ȡ������ı�
 *
 *	�����β�: ��ȡ�ı�ǩ nowTag	 ����ֵ: ��-1 ��-0 
 */
int openTextInputMode(string nowTag)
{
	string a = (string)malloc(sizeof(char) * (strlen(nowTag) + 1));
	strcpy(a, nowTag);
	int num = sizeof(TextInputOpenTag) / sizeof(char*);		//���б�ǩ���͵����� 
	char tmp;
	string tag=a;
	//����ǩ��Ŀո�ֵΪ'\0' 
	if(FindChar(' ', a, 0)!=-1)
	{
		
		tag+=FindChar(' ', a, 0);
		tmp = tag[0];
		tag[0] = 0;
	}
	//�жϱ�ǩ 
	int i;
	for(i = 0; i < num; i++)
	{
		if(strcmp(a, TextInputOpenTag[i])==0)	//����ǿ��е�һ����ǩ����1
		{		
			return 1;							//���򷵻�0
		}
	}
	free(a);
	return 0;
}
