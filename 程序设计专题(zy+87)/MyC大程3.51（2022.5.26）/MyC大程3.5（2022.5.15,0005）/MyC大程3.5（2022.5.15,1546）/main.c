//======================================================
//	main.c
//
//	��Ȩ�����ߣ��ʼ������
// 				�ƺ������Х�������� 
//
//	���һ���޸ģ�2022/5/15 
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

double windowHeight, windowWidth;						//��Ļ�߶ȺͿ�� 
double mouseX, mouseY;									//���λ��x,y 
int isClick;											//�ж��Ƿ��� 
int ithWebPage; 										//��ǰ��ҳ 
int webPageNum;											//��ǰ����ҳ�� 
page PageList[10];										//���ͬʱ֧��10����ҳ 
displayBox textBox, outputBox;							//���������� 

void myMouseEvent(int x, int y, int button, int event);	//�����Ӧ 
void myKeyboardEvent(int key, int event);				//������Ӧ 
void myCharEvent(char c);								//�ַ���Ӧ 
void myTimerEvnet(int id);

void Main()
{
	SetWindowTitle("html ������");
	SetWindowSize(22.0, 14.0);
	//InitConsole();
	InitGraphics();
	DefineColor("Gray", 0.90, 0.90, 0.90); 	
	windowHeight = GetWindowHeight();
	windowWidth = GetWindowWidth();
	
	//Ϊ������������趨��ͬĬ������ֵ 
	registerDefaultDisplayBox(windowWidth / 18, windowHeight * 0.06,
							 windowWidth * 0.4, windowHeight * 0.8, &textBox);
	registerDefaultDisplayBox(windowWidth / 18 + windowWidth * 0.5,
							  windowHeight * 0.06, windowWidth * 0.4,
							  windowHeight * 0.8, &outputBox);
	//Ĭ�ϼ�����ҳ 
	loadingPage(&PageList[0], "source/homepage.html");
	//ע����Ӧ���� 
	registerMouseEvent(myMouseEvent);
	registerKeyboardEvent(myKeyboardEvent);
	registerCharEvent(myCharEvent);
	display();
}

//�����Ӧ 
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

//������Ӧ 
void myKeyboardEvent(int key, int event)
{
	uiGetKeyboard(key, event);
	mouseX = GetMouseX();
	mouseY = GetMouseY();
	display();
}

//�ַ���Ӧ 
void myCharEvent(char c)
{
	uiGetChar(c);
	display();
}

