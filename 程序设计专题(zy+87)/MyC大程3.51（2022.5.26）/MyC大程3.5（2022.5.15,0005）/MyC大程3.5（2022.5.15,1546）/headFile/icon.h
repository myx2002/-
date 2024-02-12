//======================================================
//	icon.h
//
//	版权所有者：皇家马德里
// 				黄鸿宇、马宇啸、嵇嘉宇 
//
//	最近一次修改：2022/5/15
//
//======================================================
#ifndef __ICON_H__
#define __ICON_H__

/*
 *	函数名: drawCircleReflesh 
 *
 *	功能：画刷新按钮图标
 *
 *	接受形参：圆心坐标 x,y  相对半径 r 	返回值：无  
 */
void drawCircleReflesh(double x, double y, double r);

/*
 *	函数名: drawLeftArrow
 *
 *	功能：画"后退"按钮图标 
 *
 *	接受形参：圆心坐标 x,y  相对半径 r 	返回值：无  
 */
void drawLeftArrow(double x, double y, double r);

/*
 *	函数名: drawRightArrow
 *
 *	功能：画"前进"按钮图标 
 *
 *	接受形参：圆心坐标 x,y  相对半径 r 	返回值：无  
 */
void drawRightArrow(double x, double y, double r);

/*
 *	函数名: drawHome
 *
 *	功能：画"主页"按钮图标 
 *
 *	接受形参：圆心坐标 x,y  相对半径 r 	返回值：无  
 */
void drawHome(double x, double y, double r);

/*
 *	函数名: drawMultiplication 
 *
 *	功能：画"关闭"按钮图标 
 *
 *	接受形参：圆心坐标 x,y  半相对径 r 	返回值：无  
 */
void drawMultiplication(double x, double y, double r);

/*
 *	函数名: drawPlus
 *
 *	功能：画"新建"按钮图标 
 *
 *	接受形参：圆心坐标 x,y  相对半径 r 	返回值：无 
 */
void drawPlus(double x, double y, double r);

/*
 *	函数名: reactCircleReflesh 
 *
 *	功能：刷新 
 */
void reactCircleReflesh(); 

/*
 *	函数名: reactLeftArrow 
 *
 *	功能：后退（上一页） 
 */
void reactLeftArrow();

/*
 *	函数名: reactRightArrow 
 *
 *	功能：前进（下一页 ） 
 */
void reactRightArrow();

/*
 *	函数名: reactHome 
 *
 *	功能：返回主页 
 */
void reactHome();

/*
 *	函数名: reactPlus
 *
 *	功能：新建网页 
 */
void reactPlus();

/*
 *	函数名: permitRightArrow
 *
 *	功能：判断是否可以点击前进按钮
 *
 *	返回值：是-1 否-0 
 */
int permitRightArrow();

/*
 *	函数名: permitLeftArrow
 *
 *	功能：判断是否可以点击后退按钮 
 *
 *	返回值：是-1 否-0 
 */
int permitLeftArrow();

/*
 *	函数名: permitPlus 
 *
 *	功能：判断是否可以新建网页 
 *
 *	返回值：是-1 否-0 
 */
int permitPlus();

#endif
