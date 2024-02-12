//======================================================
//	displaybox.c
//
//	版权所有者：皇家马德里
// 				黄鸿宇、马宇啸、嵇嘉宇 
//
//	最近一次修改：2022/5/15
//
//======================================================
#include "displaybox.h"
#include "string.h"
#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"
#include "linkedlist.h"
#include "strlib.h"

enum lineLocationEnum lineLocation;		//划线位置 
enum lineTypeEnum lineType;				//线条类型 

/*
 *	函数名: registerDefaultDisplayBox
 *
 *	功能：注册一个默认类型的展示框 
 *
 *	接受形参：位置 x,y 宽和高 boxWidth,boxHeight 指定展示框 nowBox 
 */
void registerDefaultDisplayBox(double positionX, double positionY,
						double boxWidth, double boxHeight, displayBox* nowBox)
{
	nowBox->positionX = positionX;
	nowBox->positionY = positionY;
	nowBox->boxWdith = boxWidth;
	nowBox->boxHeight = boxHeight;
	nowBox->boxSideGap = boxWidth * 0.05;
	strcpy(nowBox->fontType, "宋体");
	nowBox->fontSize = 20;
	nowBox->globalFontSize = 20; 
	SetPointSize(nowBox->fontSize);
}

/*
 *	函数名: drawDisplayBox 
 *
 *	功能：绘制展示框 
 *
 *	接受形参：指定展示框 box 
 */
void drawDisplayBox(displayBox *box)
{
	SetPenColor("blue");
	drawRectangle(box->positionX, box->positionY, box->boxWdith, box->boxHeight, 0);
}

/*
 *	函数名: writeInBox
 *
 *	功能：在指定展示框打印字符串 a  
 *
 *	接受形参: 字符串 a , 展示框 nowBox  返回值: 无 
 */
void writeInBox(string a, displayBox *nowBox)
{
	//设置笔和字的相关属性 
	SetFont(nowBox->fontType); 
	SetPointSize(nowBox->fontSize);
	SetPenColor("black");
	MovePen(nowBox->nowPrintX, nowBox->nowPrintY);
	//逐字输出
	int i; 
	for(i = 0; i < strlen(a); ++i)
	{
		string tmp = NULL;
		if(a[i] > 0) 		//如果a[i]>0 此处为英文 
		{
			tmp = CharToString(a[i]);
		}
		else 				// 如果a[i]<0 则该处为中文，打印时需两格一起输出 
		{
			tmp = SubString(a, i, i + 1);
			++i; 
		}
		//判断是否溢出展示框，需要换行 
		if(nowBox->nowPrintX + TextStringWidth(tmp)
		   <=nowBox->positionX + nowBox->boxWdith - nowBox->boxSideGap)
		{
			nowBox->nowPrintX+=TextStringWidth(tmp);	//更新nowPtintX的位置 
			DrawTextString(tmp);
		}
		else
		{
			shift(nowBox);			//如果需要换行，则调用换行函数 
			DrawTextString(tmp); 
		}			
	} 
	SetPenColor("blue");
}

/*
 *	函数名: shift
 *
 *	功能：换行
 *
 *	接受形参: 指定展示框 nowBox	 返回值: 无 
 */
void shift(displayBox *nowBox)
{
	SetPointSize(nowBox->fontSize);
	nowBox->nowPrintY-=GetFontHeight(); 						//printY移下一行 
	nowBox->nowPrintX = nowBox->positionX + nowBox->boxSideGap; 	//printX倒回最左侧 
	MovePen(nowBox->nowPrintX, nowBox->nowPrintY);				//将打印位置调整到换行后的位置 
}

/*
 *	函数名: reset
 *
 *	功能：（换页时）重置输出位置于展示框起始 
 *
 *	接受形参: 指定展示框 nowBox	 返回值: 无 
 */
void reset(displayBox* nowBox)
{
	nowBox->nowPrintX = nowBox->positionX+nowBox->boxSideGap;
	nowBox->nowPrintY = nowBox->positionY+nowBox->boxHeight - 1;
}

/*
 *	函数名: drawHorizentalLine
 *
 *	功能：画分割线 
 *
 *	接受形参: 指定展示框 nowBox	 返回值: 无 
 */
void drawHorizentalLine(displayBox* nowBox)	
{
	SetPenColor("black");
	MovePen(nowBox->nowPrintX - nowBox->boxSideGap, nowBox->nowPrintY);
	DrawLine(nowBox->boxWdith, 0);
	shift(nowBox);
}

/*
 *	函数名: drawCircle
 *
 *	功能：画圆圈（用于列表） 
 *
 *	接受形参: 指定展示框 a 圆圈类型 type 返回值: 无 
 */
void drawCircle(displayBox* nowBox, int type)
{
	//圆圈圆心相当于一个字符的中心，半径相当于一个字符的一半长度 
	MovePen(nowBox->nowPrintX + TextStringWidth("a") / 2,nowBox->nowPrintY + TextStringWidth("a") / 2);
	SetPenSize(1);
	SetPenColor("black");
	if(type==1) StartFilledRegion(1);	//type=1时画实心圆 
	DrawArc(TextStringWidth("a") * 0.5, 0, 360);
	if(type==1) EndFilledRegion();
	nowBox->nowPrintX+=TextStringWidth("a");
	
}

/*
 *	函数名: drawLineWithText
 *
 *	功能：输出带有线的文本（下划线、删除线） 
 *
 *	接受形参: 指定展示框 nowBox	, 字符串 a, 线的位置 lineLocation 线的种类style
 *	返回值: 无 
 */
void drawLineWithText(displayBox* nowBox, string a, enum lineLocationEnum lineLocation,
												enum lineTypeEnum lineType, string color)
{
	//设置笔和字的相关属性 
	SetFont(nowBox->fontType); 
	SetPointSize(nowBox->fontSize);
	MovePen(nowBox->nowPrintX, nowBox->nowPrintY);
	double oldX = nowBox->nowPrintX;
	//逐字输出
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
		if(a[i] > 0) //如果a[i]>0 此处为英文 
		{
			tmp = CharToString(a[i]);
		}
		else // 如果a[i]<0 则该处为中文，打印时需两格一起输出 
		{
			tmp = SubString(a, i, i + 1);
			++i; 
		}
		//判断是否溢出展示框，需要换行 
		if(nowBox->nowPrintX + TextStringWidth(tmp)
		   <=nowBox->positionX + nowBox->boxWdith - nowBox->boxSideGap)
		{
			nowBox->nowPrintX+=TextStringWidth(tmp);
			DrawTextString(tmp);
		}
		else
		{
			shift(nowBox);//如果需要换行，则调用换行函数 
			oldX = nowBox->nowPrintX;
			DrawTextString(tmp);
			nowBox->nowPrintX+=TextStringWidth(tmp);
		}
		if(lineLocation==down)		//画下划线
		{	
			MovePen(oldX, nowBox->nowPrintY - GetFontDescent());
			if(lineType==normalLine)	//实线 
			{
				DrawLine(nowBox->nowPrintX-oldX, 0);
			}
			else if(lineType==dotLine)	//虚线 
			{
				drawDotLine((nowBox->nowPrintX) - oldX, color); 
			}
		}
		else if(lineLocation==1)	//画删除线
		{
			MovePen(oldX, nowBox->nowPrintY - GetFontDescent() + GetFontHeight() / 2);
			if(lineType==normalLine)	//实现
			{
				DrawLine(nowBox->nowPrintX - oldX, 0);
			}
			else if(lineType==dotLine)	//虚线 
			{
				drawDotLine((nowBox->nowPrintX) - oldX, color); 
			}
		}
	}
	SetPenColor("blue");	
}

/*
 *	函数名: drawDotLine
 *
 *	功能：画虚线 
 *
 *	接受形参: 指定长度 length 返回值: 无  
 */
void drawDotLine(double length, string color)
{
	int nowx = 0;
	//交替绘制有颜色的线和白色的线达到虚线效果 
	while(nowx * 0.05<=length)
	{
		if(!(nowx&1))	//当nowx是偶数时 
		{
			if(color)	//如果传入了颜色参数 
			{
				SetPenColor(color);
			}
			else		//默认颜色 
			{
				SetPenColor("black");				
			}
		}
		else			//当nowx时奇数时 
		{
			SetPenColor("white"); 
		}
		DrawLine(0.05, 0);
		nowx++;
	}
	SetPenColor("black");
}

/*
 *	函数名: resetBoxSideGap 
 *
 *	功能：根据传入的输出框结构重新设置 
 *
 *	接受形参: 指定长度 length 返回值: 无  
 */
void resetBoxSideGap(displayBox* nowBox)
{
	nowBox->nowPrintX=nowBox->positionX + nowBox->boxSideGap; //printX倒回最左侧 	
}
