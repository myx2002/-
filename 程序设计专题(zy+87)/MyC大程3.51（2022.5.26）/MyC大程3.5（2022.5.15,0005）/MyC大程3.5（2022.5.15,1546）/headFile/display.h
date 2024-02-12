//======================================================
//	display.h
//
//	版权所有者：皇家马德里
// 				黄鸿宇、马宇啸、嵇嘉宇 
//
//	最近一次修改：2022/5/1 
//
//======================================================

#ifndef __DISPLAY_H__
#define __DISPLAY_H__

/*
 *	函数名: display
 *
 *	功能：绘制浏览器
 */
void display();

/*
 *	函数名: drawButton
 *
 *	功能：绘制菜单栏 
 */
void drawMenu(); 

/*
 *	函数名: drawButton
 *
 *	功能：绘制按钮
 */
void drawButton();

/*
 *	函数名: drawTextBox
 *
 *	功能：绘制地址栏 
 */
void drawTextBox(); 

/*
 *	函数名: drawOutputBox
 *
 *	功能：打印输出框内容并在需要翻页时将该处节点作为新一页的表头 
 */
void drawOutputBox();

/*
 *	函数名: drawInputBox
 *
 *	功能：打印输入框内容并在需要翻页时将该处节点作为新一页的表头  
 */
void drawInputBox();

/*
 *	函数名: printNodeIntoTextBox
 *
 *	功能：将文件内容输出在输入框中
 *
 *	接受形参： 指向输出内容的指针 obj  返回值：无 
 */
void drawPageBar();

#endif
