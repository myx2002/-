//======================================================
//	displaybox.h
//
//	版权所有者：皇家马德里
// 				黄鸿宇、马宇啸、嵇嘉宇 
//
//  Type:ddisplaybox
//	该头文件构造了一个新类型，展示框，构造时只要提供其左下角坐标
//  与宽度，高度，其他参数默认设置，其主要功能为，接受一个字符串并
//  在屏幕上显示，显示时会根据框边界自动换行。 
//	
//	最近一次修改：2022/5/15
//======================================================

#ifndef __DISPLAYBOX_H__
#define __DISPLAYBOX_H__
#include "strlib.h"
#include "enumType.h"

//定义displayBox的结构 
typedef struct displayBox{
	double positionX, positionY;		//定义展示框左下角坐标宽度 
	double boxWdith, boxHeight; 		//定义展示框宽度与高度
	double boxSideGap;				//定义窗口显示文字距边框的距离
	double nowPrintX, nowPrintY; 	//当前字符打印位置
	int fontSize;					//当前字体大小 
	int globalFontSize;				//全局字体大小 
	char fontType[10];				//记录字体 
} displayBox;

/*
 *	函数名: registerDefaultDisplayBox
 *
 *	功能：注册一个默认类型的展示框 
 *
 *	接受形参：位置 x,y 宽和高 boxWidth,boxHeight 指定展示框 nowBox 
 */
void registerDefaultDisplayBox(double positionX, double positionY,
						double boxWidth, double boxHeight, displayBox*); 

/*
 *	函数名: drawDisplayBox 
 *
 *	功能：绘制展示框 
 *
 *	接受形参：指定展示框 box 
 */
void drawDisplaybox(displayBox* a); 

/*
 *	函数名: writeInBox
 *
 *	功能：在指定展示框打印字符串 a  
 *
 *	接受形参: 字符串 a , 展示框 nowBox  返回值: 无 
 */
void writeInBox(string, displayBox*);  

/*
 *	函数名: shift
 *
 *	功能：换行
 *
 *	接受形参: 指定展示框 nowBox	 返回值: 无 
 */
void shift(displayBox*); 

/*
 *	函数名: reset
 *
 *	功能：（换页时）重置输出位置于展示框起始 
 *
 *	接受形参: 指定展示框 nowBox	 返回值: 无 
 */
void reset(displayBox*); 

/*
 *	函数名: drawHorizentalLine
 *
 *	功能：画分割线 
 *
 *	接受形参: 指定展示框 nowBox	 返回值: 无 
 */
void drawHorizentalLine(displayBox*);

/*
 *	函数名: drawLineWithText
 *
 *	功能：输出带有线的文本（下划线、删除线） 
 *
 *	接受形参: 指定展示框 nowBox	, 字符串 a, 线的位置 lineLocation 线的种类style
 *	返回值: 无 
 */
void drawLineWithText(displayBox*,string, enum lineLocationEnum, enum lineTypeEnum, string);

/*
 *	函数名: drawCircle
 *
 *	功能：画圆圈（用于列表） 
 *
 *	接受形参: 指定展示框 a 圆圈类型 type 返回值: 无 
 */
void drawCircle(displayBox* a, int type);

/*
 *	函数名: resetBoxSideGap 
 *
 *	功能：根据传入的输出框结构重新设置 
 *
 *	接受形参: 指定长度 length 返回值: 无  
 */
void resetBoxSideGap(displayBox* now);
#endif

