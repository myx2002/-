//======================================================
//	analyses.h
//
//	版权所有者：皇家马德里
// 				黄鸿宇、马宇啸、嵇嘉宇 
//
//	最近一次修改：2022/5/15
//
//======================================================

#ifndef __WIDGET_H__
#define __WIDGET_H__

#include "strlib.h"

/*
 *	函数名: createTextWidget
 *
 *	功能：根据文本控件的区域创建一个新的 TextWidget并加入到链表
 *		  当鼠标移到文本控件的区域里时会显示相应内容 
 *
 *	接受形参: 文本内容 a, 控件类型 type, 对应内容 buffer	返回值: 无 
 */
int createTextWidgt(string a, enum printType textType, string buffer);

/*
 *	函数名: drawRoundButton
 *
 *	功能：根据draw函数指针绘制刷新、后退、前进、主页、关闭或新建按钮图标
 *		  当鼠标移动到按钮上时，按钮底部会出现灰色圆形
 *		  当鼠标点击按钮时根据相应的react函数指针执行相应函数
 *
 *	接受形参: 图标位置 x,y 图标大小 r 绘图函数指针 draw 响应函数指针react 返回值: 无 
 */
void drawRoundButton(double x, double y, double r, void(*draw)(double, double, double), void (*react)(), int (*permit)());

/*
 *	函数名: drawPage 
 *
 *	功能：写第i个网页的的网页名（实际上是第i+1个网页,因为i从0开始）
 *		  绘制关闭按钮图标 
 *		  对关闭按钮的响应（鼠标放置、点击） 
 *		  点击当前网页标签栏时返回1 
 *
 *	接受形参: 标签栏区域 x,y,w,h 网页名 name canclePage函数指针 react  当前网页i 
 *	返回值：点击 1 未点击 0 
 */
int drawPage(double x, double y, double w, double h, string name, int i);

#endif 
