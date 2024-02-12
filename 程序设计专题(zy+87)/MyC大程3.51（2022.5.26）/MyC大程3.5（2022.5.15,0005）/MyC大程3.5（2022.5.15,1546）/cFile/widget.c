//======================================================
//	widget.c
//
//	版权所有者：皇家马德里
// 				黄鸿宇、马宇啸、嵇嘉宇 
//
//	最近一次修改：2022/5/5
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

extern displayBox outputBox;			//输出框 
extern int isClick;						//是否点击 
extern double mouseX,mouseY; 			//鼠标位置 
extern page PageList[10];				//最多保存10个网页 
extern int ithWebPage; 					//当前页面
extern unfoldMenu;						//菜单是否打开  

//关于矩形属性的结构 
typedef struct squareLocation
{
	double x, y, width, height;
}squareLocation;


/*
 *	函数名: Insquare 
 *
 *	功能：判断传入的坐标是否在给定矩形区域中 
 *
 *	接受形参: 矩阵结构指针 a 鼠标位置x,y	返回值: 是-1 否-0 
 */
static int Insquare(squareLocation* a, double x, double y)
{
	//printf("%lf %lf %lf %lf\n",a->x,a->y,a->width,a->height);
	return(x>=a->x&&x<=a->x + a->width&
	   	   y>=a->y&&y<=a->y + a->height);
}

/*
 *	函数名: Inround 
 *
 *	功能：判断传入的坐标是否在给定圆形区域中 
 *
 *	接受形参: 圆心坐标 x,y 半径 r 鼠标位置x,y 	返回值: 是-1 否-0 
 */
static int Inround(double roundX, double roundY, double r, double x, double y)
{
	return sqrt(pow(x-roundX, 2) + pow(y - roundY, 2))<=r;
}

/*
 *	函数名: newLocation 
 *
 *	功能：给新的矩阵区域赋值（坐标，宽，高） 
 *
 *	接受形参: 结构指针 nowLocation, 坐标 x,y 宽 width 高 height
 *	返回值:   赋值后的结构指针 
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
 *	函数名: drawAbbr
 *
 *	功能：显示缩写内容
 *
 *	接受形参: 结构指针 nowLocation, 字符串 a  返回值：无 
 */
void drawAbbr(squareLocation* nowLocation, string a)
{
	SetPenColor("white");
	StartFilledRegion(1);		//用白色填充矩阵来把后面的文本挡住 
	drawRectangle(nowLocation->x - TextStringWidth(a) - TextStringWidth("a"), nowLocation->y,
				  TextStringWidth(a) + TextStringWidth("a"), GetFontHeight(), 0);
	EndFilledRegion();
	SetPenColor("blue");		//在白色填充矩阵上绘制被缩写内容 
	drawRectangle(nowLocation->x - TextStringWidth(a) - TextStringWidth("a"), nowLocation->y,
				  TextStringWidth(a) + TextStringWidth("a"), GetFontHeight(), 0);
	MovePen(nowLocation->x - TextStringWidth(a) - TextStringWidth("a") / 2, nowLocation->y + GetFontDescent());
	DrawTextString(a);	
}

/*
 *	函数名: createTextWidget
 *
 *	功能：根据文本控件的区域创建一个新的 TextWidget并加入到链表
 *		  当鼠标移到文本控件的区域里时会显示相应内容 
 *
 *	接受形参: 文本内容 a, 控件类型 type, 对应内容 buffer	返回值: 无 
 */
int createTextWidget(string a, enum printType textType, string buffer)
{
	//记录 
	double leftX = outputBox.nowPrintX;							//输出开头的 x值 
	double leftY = outputBox.nowPrintY;							//输出开头的 y值 
	//输出被缩写内容 or超链接，并改变 nowPrintX位置 
	if(textType==abbrText)
	{
		drawLineWithText(&outputBox, a, down, normalLine, 0);	//输出缩写文本 
	}
	else if(textType==hrefText)
	{
		drawLineWithText(&outputBox, a, down, dotLine, "blue");	//输出链接文本 
	}
	double rightX = outputBox.nowPrintX + TextStringWidth("1");	//输出结尾的 x值 
	double rightY = outputBox.nowPrintY;						//输出结尾的 y值 
	if(leftY==rightY)	//当文本仅占一行时 
	{	
		//将区域范围赋给nowLocation 
		squareLocation nowLocation = {leftX, leftY, rightX - leftX, GetFontHeight()};
		if(Insquare(&nowLocation, mouseX, mouseY))				//判断鼠标是否在区域内 
		{
			if(textType==abbrText)	
			{
				drawAbbr(&nowLocation, buffer);					//显示缩写内容		
			}
			else if(textType==hrefText&&isClick)
			{
				loadingPage(&PageList[ithWebPage], buffer);		//跳转至链接文件（网址） 
				isClick=0; 
				return 1; 
			}
		}
	}
	else 				//当文本占用多行时（分为三部分：第一行、中间行、最后一行） 
	{
		squareLocation nowLocation;
		//第一行 
		newLocation(&nowLocation, leftX, leftY, 
			outputBox.positionX + outputBox.boxWdith - outputBox.boxSideGap * 2 - leftX, GetFontHeight());
		if(Insquare(&nowLocation, mouseX, mouseY))			//判断鼠标是否在区域内
		{
			if(textType==abbrText)
			{

				drawAbbr(&nowLocation, buffer);				//显示缩写内容					
			}
			else if(textType==hrefText && isClick)
			{
				loadingPage(&PageList[ithWebPage], buffer);	//跳转至链接文件（网址） 
				isClick=0; 
				return 1; 
			}
		}
		leftY-=GetFontHeight();	//对下一行创建区域 
		//中间行 
		while(leftY!=rightY)
		{
			newLocation(&nowLocation, outputBox.positionX + outputBox.boxSideGap, leftY,
						outputBox.boxWdith - outputBox.boxSideGap * 2, GetFontHeight());
			if(Insquare(&nowLocation, mouseX, mouseY))			//判断鼠标是否在区域内
			{
				if(textType==abbrText)
				{
					drawAbbr(&nowLocation, buffer);				//显示缩写内容				
				}
				else if(textType==hrefText&&isClick)
				{
					loadingPage(&PageList[ithWebPage], buffer);	//跳转至链接文件（网址） 
					return 1; 
				}
			}
			leftY-=GetFontHeight();	//对下一行创建区域				
		}
		//最后一行 
		newLocation(&nowLocation,outputBox.positionX + outputBox.boxSideGap, leftY,
							rightX - (outputBox.positionX + outputBox.boxSideGap), GetFontHeight());
		if(Insquare(&nowLocation, mouseX, mouseY))			//判断鼠标是否在区域内
		{
			if(textType==abbrText)
			{
				drawAbbr(&nowLocation, buffer);				//显示缩写内容					
			}
			else if(textType==hrefText&&isClick)
			{
				loadingPage(&PageList[ithWebPage], buffer);	//跳转至链接文件（网址） 
				return 1; 
			}
		}
	}
	return;
}

/*
 *	函数名: drawRoundButton
 *
 *	功能：根据draw函数指针绘制刷新、后退、前进、主页、关闭或新建按钮图标
 *		  当鼠标移动到按钮上时，按钮底部会出现灰色圆形
 *		  当鼠标点击按钮时根据相应的react函数指针执行相应函数
 *
 *	接受形参: 图标位置 x,y 图标大小 r 绘图函数指针 draw 响应函数指针react 返回值: 无 
 */
void drawRoundButton(double x, double y, double r, void(*draw)(double, double, double), 
						void (*react)(), int (*permit)())
{	
	//刷新和主页按钮传入NULL，前进、后退、新建按钮传入permit函数指针 
	if(permit==NULL || (permit!=NULL&&(*permit)()))
	{
		if(Inround(x, y, r, mouseX, mouseY))//当鼠标移动至按钮图标上时 
		{
			if(isClick)						//如果点击了 
			{
				r*=0.8;						//按钮图标变小 
			}
			//绘制灰色底圆 
			MovePen(x + r, y);	
			SetPenColor("Gray");
			StartFilledRegion(1);			
			DrawArc(r, 0, 360);
			EndFilledRegion();
			
			if(isClick) 					//如果点击了
			{
				(*react)();					//调用相应函数 
			}
		}			
	}
	(*draw)(x, y, r);						//绘制按钮图标 
}

/*
 *	函数名: drawPage 
 *
 *	功能：写第i个网页的的网页名（实际上是第i+1个网页,因为i从0开始）
 *		  绘制关闭按钮图标 
 *		  对关闭按钮的响应（鼠标放置、点击） 
 *		  点击当前网页标签栏时返回1 
 *
 *	接受形参: 标签栏区域 x,y,w,h 网页名 name canclePage  当前网页i 
 *	返回值：点击 1 未点击 0 
 */
int drawPage(double x, double y, double w, double h, string name, int i)
{
 	double r = h * 0.35;
 	squareLocation nowLocation = {x, y, w, h};
	//在标签栏写网页名 
	SetPenColor("blue");	
 	MovePen(x + TextStringWidth("1"), y + GetFontHeight() / 2 - GetFontDescent());
 	DrawTextString(name);
 	//用白色矩形覆盖超出长度的网页名部分 
 	SetPenColor("white");
 	drawRectangle(x + w - r * 3.3, y, w * 10, h, 1); 
 	//如果鼠标放在关闭按钮上
	if(!unfoldMenu)						//如果菜单未打开 
	{
		if(Inround(x + w - r * 2.1, y + h / 2, r, mouseX, mouseY))
 		{
  			if(isClick)
  			{
   				r*=0.8;					//(点击）关闭图标变小 
  			}
  			SetPenColor("red");			//只要鼠标放置到关闭按钮上，按钮图标变红 
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
 	drawMultiplication(x + w - r * 2.1, y + h / 2, r);	//绘制关闭按钮 
  	SetPenColor("blue");	
 	drawRectangle(x, y, w, h, 0);						//绘制单个标签栏 
 	//当鼠标点击该标签栏时返回1，点击关闭按钮时返回2 
 	if(!unfoldMenu)										//如果菜单未打开 
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

