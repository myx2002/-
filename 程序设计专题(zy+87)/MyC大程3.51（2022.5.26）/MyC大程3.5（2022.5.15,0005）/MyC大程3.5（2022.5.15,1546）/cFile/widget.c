//======================================================
//	widget.c
//
//	��Ȩ�����ߣ��ʼ������
// 				�ƺ������Х�������� 
//
//	���һ���޸ģ�2022/5/5
//
//======================================================

#include "stdio.h"
#include "math.h"
#include "displaybox.h"
#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"
#include "page.h"
#include "enumType.h"
#include "widget.h"
#include "linkedlist.h"
#include "strlib.h"

extern displayBox outputBox;			//����� 
extern int isClick;						//�Ƿ��� 
extern double mouseX,mouseY; 			//���λ�� 
extern page PageList[10];				//��ౣ��10����ҳ 
extern int ithWebPage; 					//��ǰҳ��
extern unfoldMenu;						//�˵��Ƿ��  

//���ھ������ԵĽṹ 
typedef struct squareLocation
{
	double x, y, width, height;
}squareLocation;


/*
 *	������: Insquare 
 *
 *	���ܣ��жϴ���������Ƿ��ڸ������������� 
 *
 *	�����β�: ����ṹָ�� a ���λ��x,y	����ֵ: ��-1 ��-0 
 */
static int Insquare(squareLocation* a, double x, double y)
{
	//printf("%lf %lf %lf %lf\n",a->x,a->y,a->width,a->height);
	return(x>=a->x&&x<=a->x + a->width&
	   	   y>=a->y&&y<=a->y + a->height);
}

/*
 *	������: Inround 
 *
 *	���ܣ��жϴ���������Ƿ��ڸ���Բ�������� 
 *
 *	�����β�: Բ������ x,y �뾶 r ���λ��x,y 	����ֵ: ��-1 ��-0 
 */
static int Inround(double roundX, double roundY, double r, double x, double y)
{
	return sqrt(pow(x-roundX, 2) + pow(y - roundY, 2))<=r;
}

/*
 *	������: newLocation 
 *
 *	���ܣ����µľ�������ֵ�����꣬���ߣ� 
 *
 *	�����β�: �ṹָ�� nowLocation, ���� x,y �� width �� height
 *	����ֵ:   ��ֵ��Ľṹָ�� 
 */
void newLocation(squareLocation* nowLocation,
								double x, double y, double width, double height)
{
	nowLocation->x = x;
	nowLocation->y = y;
	nowLocation->width = width;
	nowLocation->height = height;
	//printf("%lf,%lf,%lf,%lf\n",x,y,width,height); 
}

/*
 *	������: drawAbbr
 *
 *	���ܣ���ʾ��д����
 *
 *	�����β�: �ṹָ�� nowLocation, �ַ��� a  ����ֵ���� 
 */
void drawAbbr(squareLocation* nowLocation, string a)
{
	SetPenColor("white");
	StartFilledRegion(1);		//�ð�ɫ���������Ѻ�����ı���ס 
	drawRectangle(nowLocation->x - TextStringWidth(a) - TextStringWidth("a"), nowLocation->y,
				  TextStringWidth(a) + TextStringWidth("a"), GetFontHeight(), 0);
	EndFilledRegion();
	SetPenColor("blue");		//�ڰ�ɫ�������ϻ��Ʊ���д���� 
	drawRectangle(nowLocation->x - TextStringWidth(a) - TextStringWidth("a"), nowLocation->y,
				  TextStringWidth(a) + TextStringWidth("a"), GetFontHeight(), 0);
	MovePen(nowLocation->x - TextStringWidth(a) - TextStringWidth("a") / 2, nowLocation->y + GetFontDescent());
	DrawTextString(a);	
}

/*
 *	������: createTextWidget
 *
 *	���ܣ������ı��ؼ������򴴽�һ���µ� TextWidget�����뵽����
 *		  ������Ƶ��ı��ؼ���������ʱ����ʾ��Ӧ���� 
 *
 *	�����β�: �ı����� a, �ؼ����� type, ��Ӧ���� buffer	����ֵ: �� 
 */
int createTextWidget(string a, enum printType textType, string buffer)
{
	//��¼ 
	double leftX = outputBox.nowPrintX;							//�����ͷ�� xֵ 
	double leftY = outputBox.nowPrintY;							//�����ͷ�� yֵ 
	//�������д���� or�����ӣ����ı� nowPrintXλ�� 
	if(textType==abbrText)
	{
		drawLineWithText(&outputBox, a, down, normalLine, 0);	//�����д�ı� 
	}
	else if(textType==hrefText)
	{
		drawLineWithText(&outputBox, a, down, dotLine, "blue");	//��������ı� 
	}
	double rightX = outputBox.nowPrintX + TextStringWidth("1");	//�����β�� xֵ 
	double rightY = outputBox.nowPrintY;						//�����β�� yֵ 
	if(leftY==rightY)	//���ı���ռһ��ʱ 
	{	
		//������Χ����nowLocation 
		squareLocation nowLocation = {leftX, leftY, rightX - leftX, GetFontHeight()};
		if(Insquare(&nowLocation, mouseX, mouseY))				//�ж�����Ƿ��������� 
		{
			if(textType==abbrText)	
			{
				drawAbbr(&nowLocation, buffer);					//��ʾ��д����		
			}
			else if(textType==hrefText&&isClick)
			{
				loadingPage(&PageList[ithWebPage], buffer);		//��ת�������ļ�����ַ�� 
				isClick=0; 
				return 1; 
			}
		}
	}
	else 				//���ı�ռ�ö���ʱ����Ϊ�����֣���һ�С��м��С����һ�У� 
	{
		squareLocation nowLocation;
		//��һ�� 
		newLocation(&nowLocation, leftX, leftY, 
			outputBox.positionX + outputBox.boxWdith - outputBox.boxSideGap * 2 - leftX, GetFontHeight());
		if(Insquare(&nowLocation, mouseX, mouseY))			//�ж�����Ƿ���������
		{
			if(textType==abbrText)
			{

				drawAbbr(&nowLocation, buffer);				//��ʾ��д����					
			}
			else if(textType==hrefText && isClick)
			{
				loadingPage(&PageList[ithWebPage], buffer);	//��ת�������ļ�����ַ�� 
				isClick=0; 
				return 1; 
			}
		}
		leftY-=GetFontHeight();	//����һ�д������� 
		//�м��� 
		while(leftY!=rightY)
		{
			newLocation(&nowLocation, outputBox.positionX + outputBox.boxSideGap, leftY,
						outputBox.boxWdith - outputBox.boxSideGap * 2, GetFontHeight());
			if(Insquare(&nowLocation, mouseX, mouseY))			//�ж�����Ƿ���������
			{
				if(textType==abbrText)
				{
					drawAbbr(&nowLocation, buffer);				//��ʾ��д����				
				}
				else if(textType==hrefText&&isClick)
				{
					loadingPage(&PageList[ithWebPage], buffer);	//��ת�������ļ�����ַ�� 
					return 1; 
				}
			}
			leftY-=GetFontHeight();	//����һ�д�������				
		}
		//���һ�� 
		newLocation(&nowLocation,outputBox.positionX + outputBox.boxSideGap, leftY,
							rightX - (outputBox.positionX + outputBox.boxSideGap), GetFontHeight());
		if(Insquare(&nowLocation, mouseX, mouseY))			//�ж�����Ƿ���������
		{
			if(textType==abbrText)
			{
				drawAbbr(&nowLocation, buffer);				//��ʾ��д����					
			}
			else if(textType==hrefText&&isClick)
			{
				loadingPage(&PageList[ithWebPage], buffer);	//��ת�������ļ�����ַ�� 
				return 1; 
			}
		}
	}
	return;
}

/*
 *	������: drawRoundButton
 *
 *	���ܣ�����draw����ָ�����ˢ�¡����ˡ�ǰ������ҳ���رջ��½���ťͼ��
 *		  ������ƶ�����ť��ʱ����ť�ײ�����ֻ�ɫԲ��
 *		  ���������ťʱ������Ӧ��react����ָ��ִ����Ӧ����
 *
 *	�����β�: ͼ��λ�� x,y ͼ���С r ��ͼ����ָ�� draw ��Ӧ����ָ��react ����ֵ: �� 
 */
void drawRoundButton(double x, double y, double r, void(*draw)(double, double, double), 
						void (*react)(), int (*permit)())
{	
	//ˢ�º���ҳ��ť����NULL��ǰ�������ˡ��½���ť����permit����ָ�� 
	if(permit==NULL || (permit!=NULL&&(*permit)()))
	{
		if(Inround(x, y, r, mouseX, mouseY))//������ƶ�����ťͼ����ʱ 
		{
			if(isClick)						//�������� 
			{
				r*=0.8;						//��ťͼ���С 
			}
			//���ƻ�ɫ��Բ 
			MovePen(x + r, y);	
			SetPenColor("Gray");
			StartFilledRegion(1);			
			DrawArc(r, 0, 360);
			EndFilledRegion();
			
			if(isClick) 					//��������
			{
				(*react)();					//������Ӧ���� 
			}
		}			
	}
	(*draw)(x, y, r);						//���ư�ťͼ�� 
}

/*
 *	������: drawPage 
 *
 *	���ܣ�д��i����ҳ�ĵ���ҳ����ʵ�����ǵ�i+1����ҳ,��Ϊi��0��ʼ��
 *		  ���ƹرհ�ťͼ�� 
 *		  �Թرհ�ť����Ӧ�������á������ 
 *		  �����ǰ��ҳ��ǩ��ʱ����1 
 *
 *	�����β�: ��ǩ������ x,y,w,h ��ҳ�� name canclePage  ��ǰ��ҳi 
 *	����ֵ����� 1 δ��� 0 
 */
int drawPage(double x, double y, double w, double h, string name, int i)
{
 	double r = h * 0.35;
 	squareLocation nowLocation = {x, y, w, h};
	//�ڱ�ǩ��д��ҳ�� 
	SetPenColor("blue");	
 	MovePen(x + TextStringWidth("1"), y + GetFontHeight() / 2 - GetFontDescent());
 	DrawTextString(name);
 	//�ð�ɫ���θ��ǳ������ȵ���ҳ������ 
 	SetPenColor("white");
 	drawRectangle(x + w - r * 3.3, y, w * 10, h, 1); 
 	//��������ڹرհ�ť��
	if(!unfoldMenu)						//����˵�δ�� 
	{
		if(Inround(x + w - r * 2.1, y + h / 2, r, mouseX, mouseY))
 		{
  			if(isClick)
  			{
   				r*=0.8;					//(������ر�ͼ���С 
  			}
  			SetPenColor("red");			//ֻҪ�����õ��رհ�ť�ϣ���ťͼ���� 
 		}
 		else
 		{
 			SetPenColor("blue");
		}
	} 
	else
	{
		SetPenColor("blue");
	}
 	drawMultiplication(x + w - r * 2.1, y + h / 2, r);	//���ƹرհ�ť 
  	SetPenColor("blue");	
 	drawRectangle(x, y, w, h, 0);						//���Ƶ�����ǩ�� 
 	//��������ñ�ǩ��ʱ����1������رհ�ťʱ����2 
 	if(!unfoldMenu)										//����˵�δ�� 
 	{
 		if(Inround(x + w - r * 2.1, y + h / 2, r * 1.25, mouseX, mouseY) && isClick)
		{
			isClick=0;
			return 2;
		}
 		if(Insquare(&nowLocation, mouseX, mouseY) && 
		 	!Inround(x + w * 0.9, y + h / 2, r, mouseX, mouseY) && isClick)
 		{
 			return 1;
		}
 	}
 	return 0;
}

