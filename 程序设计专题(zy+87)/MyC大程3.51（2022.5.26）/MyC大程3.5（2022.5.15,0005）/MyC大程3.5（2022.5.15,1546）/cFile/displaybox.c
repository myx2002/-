//======================================================
//	displaybox.c
//
//	��Ȩ�����ߣ��ʼ������
// 				�ƺ������Х�������� 
//
//	���һ���޸ģ�2022/5/15
//
//======================================================
#include "displaybox.h"
#include "string.h"
#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"
#include "linkedlist.h"
#include "strlib.h"

enum lineLocationEnum lineLocation;		//����λ�� 
enum lineTypeEnum lineType;				//�������� 

/*
 *	������: registerDefaultDisplayBox
 *
 *	���ܣ�ע��һ��Ĭ�����͵�չʾ�� 
 *
 *	�����βΣ�λ�� x,y ��͸� boxWidth,boxHeight ָ��չʾ�� nowBox 
 */
void registerDefaultDisplayBox(double positionX, double positionY,
						double boxWidth, double boxHeight, displayBox* nowBox)
{
	nowBox->positionX = positionX;
	nowBox->positionY = positionY;
	nowBox->boxWdith = boxWidth;
	nowBox->boxHeight = boxHeight;
	nowBox->boxSideGap = boxWidth * 0.05;
	strcpy(nowBox->fontType, "����");
	nowBox->fontSize = 20;
	nowBox->globalFontSize = 20; 
	SetPointSize(nowBox->fontSize);
}

/*
 *	������: drawDisplayBox 
 *
 *	���ܣ�����չʾ�� 
 *
 *	�����βΣ�ָ��չʾ�� box 
 */
void drawDisplayBox(displayBox *box)
{
	SetPenColor("blue");
	drawRectangle(box->positionX, box->positionY, box->boxWdith, box->boxHeight, 0);
}

/*
 *	������: writeInBox
 *
 *	���ܣ���ָ��չʾ���ӡ�ַ��� a  
 *
 *	�����β�: �ַ��� a , չʾ�� nowBox  ����ֵ: �� 
 */
void writeInBox(string a, displayBox *nowBox)
{
	//���ñʺ��ֵ�������� 
	SetFont(nowBox->fontType); 
	SetPointSize(nowBox->fontSize);
	SetPenColor("black");
	MovePen(nowBox->nowPrintX, nowBox->nowPrintY);
	//�������
	int i; 
	for(i = 0; i < strlen(a); ++i)
	{
		string tmp = NULL;
		if(a[i] > 0) 		//���a[i]>0 �˴�ΪӢ�� 
		{
			tmp = CharToString(a[i]);
		}
		else 				// ���a[i]<0 ��ô�Ϊ���ģ���ӡʱ������һ����� 
		{
			tmp = SubString(a, i, i + 1);
			++i; 
		}
		//�ж��Ƿ����չʾ����Ҫ���� 
		if(nowBox->nowPrintX + TextStringWidth(tmp)
		   <=nowBox->positionX + nowBox->boxWdith - nowBox->boxSideGap)
		{
			nowBox->nowPrintX+=TextStringWidth(tmp);	//����nowPtintX��λ�� 
			DrawTextString(tmp);
		}
		else
		{
			shift(nowBox);			//�����Ҫ���У�����û��к��� 
			DrawTextString(tmp); 
		}			
	} 
	SetPenColor("blue");
}

/*
 *	������: shift
 *
 *	���ܣ�����
 *
 *	�����β�: ָ��չʾ�� nowBox	 ����ֵ: �� 
 */
void shift(displayBox *nowBox)
{
	SetPointSize(nowBox->fontSize);
	nowBox->nowPrintY-=GetFontHeight(); 						//printY����һ�� 
	nowBox->nowPrintX = nowBox->positionX + nowBox->boxSideGap; 	//printX��������� 
	MovePen(nowBox->nowPrintX, nowBox->nowPrintY);				//����ӡλ�õ��������к��λ�� 
}

/*
 *	������: reset
 *
 *	���ܣ�����ҳʱ���������λ����չʾ����ʼ 
 *
 *	�����β�: ָ��չʾ�� nowBox	 ����ֵ: �� 
 */
void reset(displayBox* nowBox)
{
	nowBox->nowPrintX = nowBox->positionX+nowBox->boxSideGap;
	nowBox->nowPrintY = nowBox->positionY+nowBox->boxHeight - 1;
}

/*
 *	������: drawHorizentalLine
 *
 *	���ܣ����ָ��� 
 *
 *	�����β�: ָ��չʾ�� nowBox	 ����ֵ: �� 
 */
void drawHorizentalLine(displayBox* nowBox)	
{
	SetPenColor("black");
	MovePen(nowBox->nowPrintX - nowBox->boxSideGap, nowBox->nowPrintY);
	DrawLine(nowBox->boxWdith, 0);
	shift(nowBox);
}

/*
 *	������: drawCircle
 *
 *	���ܣ���ԲȦ�������б� 
 *
 *	�����β�: ָ��չʾ�� a ԲȦ���� type ����ֵ: �� 
 */
void drawCircle(displayBox* nowBox, int type)
{
	//ԲȦԲ���൱��һ���ַ������ģ��뾶�൱��һ���ַ���һ�볤�� 
	MovePen(nowBox->nowPrintX + TextStringWidth("a") / 2,nowBox->nowPrintY + TextStringWidth("a") / 2);
	SetPenSize(1);
	SetPenColor("black");
	if(type==1) StartFilledRegion(1);	//type=1ʱ��ʵ��Բ 
	DrawArc(TextStringWidth("a") * 0.5, 0, 360);
	if(type==1) EndFilledRegion();
	nowBox->nowPrintX+=TextStringWidth("a");
	
}

/*
 *	������: drawLineWithText
 *
 *	���ܣ���������ߵ��ı����»��ߡ�ɾ���ߣ� 
 *
 *	�����β�: ָ��չʾ�� nowBox	, �ַ��� a, �ߵ�λ�� lineLocation �ߵ�����style
 *	����ֵ: �� 
 */
void drawLineWithText(displayBox* nowBox, string a, enum lineLocationEnum lineLocation,
												enum lineTypeEnum lineType, string color)
{
	//���ñʺ��ֵ�������� 
	SetFont(nowBox->fontType); 
	SetPointSize(nowBox->fontSize);
	MovePen(nowBox->nowPrintX, nowBox->nowPrintY);
	double oldX = nowBox->nowPrintX;
	//�������
	int i; 
	for(i = 0; i < strlen(a); ++i)
	{
		if(color!=NULL)
		{
			SetPenColor(color);
		}
		else
		{
			SetPenColor("black");		
		}
		MovePen(nowBox->nowPrintX, nowBox->nowPrintY);
		string tmp = NULL;
		oldX=nowBox->nowPrintX; 
		if(a[i] > 0) //���a[i]>0 �˴�ΪӢ�� 
		{
			tmp = CharToString(a[i]);
		}
		else // ���a[i]<0 ��ô�Ϊ���ģ���ӡʱ������һ����� 
		{
			tmp = SubString(a, i, i + 1);
			++i; 
		}
		//�ж��Ƿ����չʾ����Ҫ���� 
		if(nowBox->nowPrintX + TextStringWidth(tmp)
		   <=nowBox->positionX + nowBox->boxWdith - nowBox->boxSideGap)
		{
			nowBox->nowPrintX+=TextStringWidth(tmp);
			DrawTextString(tmp);
		}
		else
		{
			shift(nowBox);//�����Ҫ���У�����û��к��� 
			oldX = nowBox->nowPrintX;
			DrawTextString(tmp);
			nowBox->nowPrintX+=TextStringWidth(tmp);
		}
		if(lineLocation==down)		//���»���
		{	
			MovePen(oldX, nowBox->nowPrintY - GetFontDescent());
			if(lineType==normalLine)	//ʵ�� 
			{
				DrawLine(nowBox->nowPrintX-oldX, 0);
			}
			else if(lineType==dotLine)	//���� 
			{
				drawDotLine((nowBox->nowPrintX) - oldX, color); 
			}
		}
		else if(lineLocation==1)	//��ɾ����
		{
			MovePen(oldX, nowBox->nowPrintY - GetFontDescent() + GetFontHeight() / 2);
			if(lineType==normalLine)	//ʵ��
			{
				DrawLine(nowBox->nowPrintX - oldX, 0);
			}
			else if(lineType==dotLine)	//���� 
			{
				drawDotLine((nowBox->nowPrintX) - oldX, color); 
			}
		}
	}
	SetPenColor("blue");	
}

/*
 *	������: drawDotLine
 *
 *	���ܣ������� 
 *
 *	�����β�: ָ������ length ����ֵ: ��  
 */
void drawDotLine(double length, string color)
{
	int nowx = 0;
	//�����������ɫ���ߺͰ�ɫ���ߴﵽ����Ч�� 
	while(nowx * 0.05<=length)
	{
		if(!(nowx&1))	//��nowx��ż��ʱ 
		{
			if(color)	//�����������ɫ���� 
			{
				SetPenColor(color);
			}
			else		//Ĭ����ɫ 
			{
				SetPenColor("black");				
			}
		}
		else			//��nowxʱ����ʱ 
		{
			SetPenColor("white"); 
		}
		DrawLine(0.05, 0);
		nowx++;
	}
	SetPenColor("black");
}

/*
 *	������: resetBoxSideGap 
 *
 *	���ܣ����ݴ���������ṹ�������� 
 *
 *	�����β�: ָ������ length ����ֵ: ��  
 */
void resetBoxSideGap(displayBox* nowBox)
{
	nowBox->nowPrintX=nowBox->positionX + nowBox->boxSideGap; //printX��������� 	
}
