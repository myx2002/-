//======================================================
//	main.c
//
//	版权所有者：皇家马德里
// 				黄鸿宇、马宇啸、嵇嘉宇 
//
//	最近一次修改：2022/5/15 
//
//======================================================

#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"
#include "linkedlist.h"
#include "string.h"

#include "analysis.h"
#include "displaybox.h" 
#include "display.h"
#include "page.h"

double windowHeight, windowWidth;						//屏幕高度和宽度 
double mouseX, mouseY;									//鼠标位置x,y 
int isClick;											//判断是否点击 
int ithWebPage; 										//当前网页 
int webPageNum;											//当前总网页数 
page PageList[10];										//最多同时支持10个网页 
displayBox textBox, outputBox;							//输入框、输出框 

void myMouseEvent(int x, int y, int button, int event);	//鼠标响应 
void myKeyboardEvent(int key, int event);				//键盘响应 
void myCharEvent(char c);								//字符响应 
void myTimerEvnet(int id);

void Main()
{
	SetWindowTitle("html 解析器");
	SetWindowSize(22.0, 14.0);
	//InitConsole();
	InitGraphics();
	DefineColor("Gray", 0.90, 0.90, 0.90); 	
	windowHeight = GetWindowHeight();
	windowWidth = GetWindowWidth();
	
	//为输入框和输出框设定不同默认属性值 
	registerDefaultDisplayBox(windowWidth / 18, windowHeight * 0.06,
							 windowWidth * 0.4, windowHeight * 0.8, &textBox);
	registerDefaultDisplayBox(windowWidth / 18 + windowWidth * 0.5,
							  windowHeight * 0.06, windowWidth * 0.4,
							  windowHeight * 0.8, &outputBox);
	//默认加载主页 
	loadingPage(&PageList[0], "source/homepage.html");
	//注册响应函数 
	registerMouseEvent(myMouseEvent);
	registerKeyboardEvent(myKeyboardEvent);
	registerCharEvent(myCharEvent);
	display();
}

//鼠标响应 
void myMouseEvent(int x , int y, int button, int event)
{
	uiGetMouse(x, y, button, event);
	mouseX = ScaleXInches(x);
	mouseY = ScaleYInches(y);
	if(button==LEFT_BUTTON && event==KEY_DOWN)
	{
		isClick = 1;
	}
	else
	{
		isClick = 0;
	} 
	display();
}

//键盘响应 
void myKeyboardEvent(int key, int event)
{
	uiGetKeyboard(key, event);
	mouseX = GetMouseX();
	mouseY = GetMouseY();
	display();
}

//字符响应 
void myCharEvent(char c)
{
	uiGetChar(c);
	display();
}

