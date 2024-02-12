//======================================================
//	analysisText.c
//
//	��Ȩ�����ߣ��ʼ������
// 				�ƺ������Х�������� 
//
//	���һ���޸ģ�2022/5/15
//
//======================================================
#include "analysisText.h"
#include "strlib.h"
#include "string.h" 
#include "display.h"
#include "displaybox.h"
#include "widget.h"
#include "enumType.h"
#include "page.h"

extern displayBox outputBox;		//�����
extern page PageList[10];			//��ౣ��10����ҳ 
extern int ithWebPage; 				//��ǰ��ҳ 
extern char buffer[100];			//�洢�ı����� 
extern textType;					//ö���ı����� 
extern lineLocation;				//ö�ٻ���λ�� 
extern lineType;					//ö���������� 

/*
 *	������: deleteChar
 *
 *	���ܣ�ɾ��ָ��������ַ� 
 *
 *	�����β�: �������ַ��� a, ��ʼ i, j	����ֵ: �� 
 */
static void deleteChar(string a, int i, int j)
{
	int length = StringLength(a);
	if(i > length)
	{
		return;						//��ָ���������ַ�����Ч����ʱ���� 
	}
	else if(j > length)			
	{
		a[i] = 0;					//��ָ����β������Ч����ʱֱ�ӽ������Ϊ'\0' 
	}
	else
	{
		int p;
		for(p = j + 1; p<=strlen(a); p++)
		{
			a[i + p - j - 1] = a[p];//����ɾ��ʱͨ��������ַ���ǰƽ��ʵ�� 
		}
	}
}

/*
 *	������: dealText
 *
 *	���ܣ������ͬ���͵��ı� 
 *
 *	�����β�: ����ַ��� a	����ֵ: �� 
 */
int dealText(string a)
{
	if(textType==titleText)		//�����ı� 
	{
		writeInPageName(&PageList[ithWebPage], a);
		textType = normalText;
		return 0;
	}
	int blank = 0;	//�жϿո� 
	int select = 0;	//�жϱ����ַ� 
	//ɾ������ո�Ѱ�ұ����ַ� 
	int i;
	for(i = 0;i < StringLength(a); i++)
	{
		if(blank&&a[i]==' ')
		{
			deleteChar(a, i, i);
			i--; 
		}
		else if(a[i]==' ')
		{
			blank = 1;			//ɾ������Ŀո� 
		}
		else if(a[i]!=' ')
		{
			blank = 0;			//�ָ�blank״̬ 
		}
		
		if(a[i]=='&')			//�����ַ�����&��ͷ��&Ϊ�����ַ� 
		{
			select = 1;			//���뱣���ַ�״̬ 
		}
		if(select==1)
		{
			//�жϱ����ַ���ɾ��ʵ���ַ� 
			if(a[i+1]=='l' && a[i+2]=='t')						//�ж�lt���'<'
			{
				a[i] = '<';
				deleteChar(a, i + 1, i + 2);						
			}
			else if(a[i+1]=='#' && a[i+2]=='6' && a[i+3]=='0')  //�ж�#60���'<'
			{
				a[i] = '<';
				deleteChar(a, i + 1, i + 3);
			}
			else if(a[i+1]=='g' && a[i+2]=='t')					//�ж�gt���'>'
			{
				a[i] = '>';
				deleteChar(a,i+1,i+2);						
			}
			else if(a[i + 1]=='#' && a[i + 2]=='6' && a[i + 3]=='2')	//�ж�#62���'>'
			{
				a[i] = '>';
				deleteChar(a, i + 1, i + 3);	
			}
			else if(a[i + 1]=='n' && a[i + 2]=='b' && 
					a[i + 3]=='s' && a[i + 4]=='p')  					//�ж�nbsp���' ' ���յ��ո�����Ӱ�� 
			{
				a[i] = ' ';
				deleteChar(a, i + 1, i + 4);
			}
			else if(a[i + 1]=='#' && a[i + 2]=='1' && 
					a[i + 3]=='6' && a[i + 4]=='0')						//�ж�#160���' ' ���յ��ո�����Ӱ�� 
			{
				a[i] = ' ';
				deleteChar(a, i + 1, i + 4);	
			}
			else if(a[i + 1]=='a' && a[i + 2]=='m' && a[i + 3]=='p')	//�ж�amp�����'&' 
			{
				a[i] = '&';
				deleteChar(a, i + 1, i + 3);								
			}
			else if(a[i + 1]=='#' && a[i + 2]=='3' && a[i + 3]=='8')	//�ж�#38�����'&' 
			{
				a[i] = '&';
				deleteChar(a, i + 1, i + 3);
				}
			else if(a[i + 1]=='t' && a[i + 2]=='i' &&
					a[i + 3]=='m' && a[i + 4]=='e' && a[i + 5]=='s')	//�ж�times,���'*' 
			{
				a[i] = '*';
				deleteChar(a, i + 1, i + 5);
			}
			else if(a[i + 1]=='#' && a[i + 2]=='2' &&
					a[i + 3]=='1' && a[i + 4]=='5')						//�ж�#215�����'*' 
			{
				a[i] = '*';
				deleteChar(a, i + 1, i + 4);	
			}
			else if(a[i + 1]=='d' && a[i + 2]=='i'&& 
					a[i + 3]=='v' && a[i + 4]=='i'&&
					a[i + 5]=='d' && a[i + 6]=='e')						//�ж�divide,���'/' 
			{
				a[i] = '/';
				deleteChar(a, i + 1, i + 6);	
			}
			else if(a[i + 1]=='#' && a[i + 2]=='2' && 
					a[i + 3]=='4' && a[i + 4]=='7')						//�ж�#247�����'/' 
			{
				a[i] = '/';
				deleteChar(a, i + 1, i + 4);
			}
		}
		select = 0;					//����״̬ 
	}
	if(textType==normalText)		//��������Ǳ�ǩ�ı� 
	{
		writeInBox(a, &outputBox);	
	}
	else if(textType==insText)		//�»��� 
	{
		drawLineWithText(&outputBox, a, 0, 0, NULL);	 
		textType = normalText;
	}
	else if(textType==delText)		//ɾ���� 
	{
		drawLineWithText(&outputBox, a, 1, 0, NULL);
		textType = normalText;
	}
	else if(textType==abbrText)		//��д 
	{
		createTextWidget(a, abbrText, buffer);
		textType = normalText;
	}
	else if(textType==hrefText)		//������ 
	{
		textType = normalText;
		string tmp = buffer;
		if(createTextWidget(a, hrefText, tmp))	//������˴����ĳ����ӵ�����󷵻�1 
		{
			return 1;
		}
	}
	return 0;
}
